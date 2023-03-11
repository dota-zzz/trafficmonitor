#include "trafficmonitor.h"
#include "HlayoutPair.h"
#include <QAction>
#include <QActionGroup>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMenu>
#include <QMouseEvent>
#include <iostream>

trafficmonitor::trafficmonitor(QWidget *parent) {
  read_json();
  create_ui();
  create_logic();
}

trafficmonitor::~trafficmonitor() {}

void trafficmonitor::create_ui() {
  // 设置无框
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  init_label(sent_speed, "0.00", up_color);
  init_label(received_speed, "0.00", down_color);
  init_label(sent_unit, "KB/S", up_color);
  init_label(received_unit, "KB/S", down_color);
  init_label(download, "下载:", down_color);
  init_label(upload, "上传:", up_color);
  hlist1 = make_hwidgetlist(upload, sent_speed, sent_unit);
  hlist2 = make_hwidgetlist(download, received_speed, received_unit);

  hlist1->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  hlist2->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  up_down_layout = new QHBoxLayout;
  up_down_layout->addWidget(hlist1);
  up_down_layout->addWidget(hlist2);

  mainwindow_layout = new QVBoxLayout;
  mainwindow_layout->addLayout(up_down_layout);
  if (is_exchange_up_down) {
    exchange_up_down();
  }
  if (is_showing_more_info) {
    show_more_info();
  } else {
    hide_more_info();
  }
  setLayout(mainwindow_layout);

  hoverwidget = new HoverWidget(&networkport, 0);
  hoverwidget->hide();
}

void trafficmonitor::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_dragPosition = event->globalPos() - frameGeometry().topLeft();
    event->accept();
  }
  if (event->button() == Qt::RightButton) {
    if (!hoverwidget->isHidden()) {
      hoverwidget->hide();
    }
    menu->exec(event->globalPos());
  }
}

void trafficmonitor::mouseMoveEvent(QMouseEvent *event) {
  QPoint currentPos = event->pos();
  if (currentPos != m_Position) {
    if (!hoverwidget->isHidden()) {
      hoverwidget->hide();
    }
    hoverwidget->reset_timer();
    m_Position = QCursor::pos();
  }
  if (event->buttons() & Qt::LeftButton & (!is_lock_window_pos)) {
    QPoint new_position = event->globalPos() - m_dragPosition;
    if (!allow_to_exceed) {
      QRect screen_rect = QGuiApplication::primaryScreen()->geometry();
      if (new_position.x() < screen_rect.left()) {
        new_position.setX(screen_rect.left());
      } else if (new_position.x() + width() > screen_rect.right()) {
        new_position.setX(screen_rect.right() - width());
      }
      if (new_position.y() < screen_rect.top()) {
        new_position.setY(screen_rect.top());
      } else if (new_position.y() + height() > screen_rect.bottom()) {
        new_position.setY(screen_rect.bottom() - height());
      }
    }

    move(new_position);
    event->accept();
  }
}

void trafficmonitor::create_logic() {

  // 网口对象设置回调函数
  networkport.setMessageChanged(
      std::bind(&trafficmonitor::MessageChanged, this));
  networkport.set_getting_speed();
  // 设置鼠标持续追踪
  setMouseTracking(true);
  // 设置悬挂显示信息窗口回调函数
  hoverwidget->setOnHoverWidgetShow(
      std::bind(&trafficmonitor::HoverWidgetShow, this));
  // 右键菜单
  always_top = new QAction("总是置顶");
  lshow_more_info = new QAction("显示更多信息");
  lock_window_pos = new QAction("锁定窗口位置");
  QAction *mainwindow_show = new QAction("显示主窗口");
  always_top->setCheckable(true);
  lshow_more_info->setCheckable(true);
  lock_window_pos->setCheckable(true);
  mainwindow_show->setCheckable(true);
  always_top->setChecked(is_always_top);
  lshow_more_info->setChecked(is_showing_more_info);
  lock_window_pos->setChecked(is_lock_window_pos);
  mainwindow_show->setChecked(is_mainwindow_show);
  connect(always_top, &QAction::triggered, this,
          &trafficmonitor::change_over_always_top);
  connect(lock_window_pos, &QAction::triggered, [&]() {
    if (is_lock_window_pos) {
      is_lock_window_pos = false;
    } else {
      is_lock_window_pos = true;
    }
  });
  connect(lshow_more_info, &QAction::triggered, [&]() {
    if (is_showing_more_info) {
      is_showing_more_info = false;
      hide_more_info();
    } else {
      is_showing_more_info = true;
      show_more_info();
    }
  });
  // 子菜单1
  menu = new QMenu(this);
  auto port_menu_ = new QMenu("选择网络连接", this);
  auto port_name_ = networkport.get_port_name();
  QActionGroup *port_group = new QActionGroup(this);
  port_group->setExclusive(true);
  for (auto i = 0; i < port_name_.size(); ++i) {
    QAction *port_act = new QAction(QString::fromStdString(port_name_[i]));
    connect(port_act, &QAction::triggered, [&]() { set_port_num(i); });
    port_menu_->addAction(port_act);
    port_group->addAction(port_act);
    port_act->setCheckable(true);
    port_act->setChecked(false);
  }
  QAction *port_auto = new QAction("自动选择");
  connect(port_auto, &QAction::triggered, [&]() { set_port_num(-1); });
  port_menu_->addAction(port_auto);
  port_group->addAction(port_auto);
  port_auto->setCheckable(true);
  port_auto->setChecked(true);

  // 子菜单2
  auto opacity_menu_ = new QMenu("窗口不透明度", this);
  QAction *upellucidity_100 = new QAction("100%");
  QAction *upellucidity_80 = new QAction("80%");
  QAction *upellucidity_60 = new QAction("60%");
  QAction *upellucidity_40 = new QAction("40%");
  QActionGroup *upellucidity_group = new QActionGroup(this);
  upellucidity_group->addAction(upellucidity_100);
  upellucidity_group->addAction(upellucidity_80);
  upellucidity_group->addAction(upellucidity_60);
  upellucidity_group->addAction(upellucidity_40);
  upellucidity_100->setCheckable(true);
  upellucidity_80->setCheckable(true);
  upellucidity_60->setCheckable(true);
  upellucidity_40->setCheckable(true);
  switch (int(upellucidity * 10)) {
  case 10: {
    upellucidity_100->setChecked(true);
    setWindowOpacity(1.0);
    break;
  }
  case 8: {
    upellucidity_80->setChecked(true);
    setWindowOpacity(8.0);
    break;
  }
  case 6: {
    upellucidity_60->setChecked(true);
    setWindowOpacity(6.0);
    break;
  }
  case 4: {
    upellucidity_40->setChecked(true);
    setWindowOpacity(4.0);
    break;
  }
  default:
    break;
  }
  opacity_menu_->addAction(upellucidity_100);
  opacity_menu_->addAction(upellucidity_80);
  opacity_menu_->addAction(upellucidity_60);
  opacity_menu_->addAction(upellucidity_40);
  connect(upellucidity_100, &QAction::triggered,
          [&]() { set_upellucidity(1.0); });
  connect(upellucidity_80, &QAction::triggered,
          [&]() { set_upellucidity(0.8); });
  connect(upellucidity_60, &QAction::triggered,
          [&]() { set_upellucidity(0.6); });
  connect(upellucidity_40, &QAction::triggered,
          [&]() { set_upellucidity(0.4); });
  // 子菜单3
  auto other_menu_ = new QMenu("其他功能", this);
  other_menu_->addAction("更换皮肤");
  other_menu_->addAction("更换通知图标");
  other_menu_->addAction("允许超出屏幕边界");
  other_menu_->addAction("历史流量统计");
  other_menu_->addAction("插件管理");
  // 子菜单4
  auto help_menu_ = new QMenu("帮助", this);
  help_menu_->addAction("帮助");
  help_menu_->addAction("常见问腿");
  help_menu_->addAction("更新日志");
  help_menu_->addAction("关于");
  help_menu_->addAction("检查更新");

  menu->addMenu(port_menu_);
  menu->addAction("连接详情", [&]() {
    connect_details = new ConnectDetails(
        connect_details, networkport.get_port_details(port_num));
    connect_details->setAttribute(Qt::WA_DeleteOnClose);
    connect_details->show();
  });
  menu->addAction(always_top);
  menu->addAction(lshow_more_info);
  menu->addAction(lock_window_pos);
  menu->addAction("显示任务栏窗口");
  menu->addAction(mainwindow_show);
  menu->addMenu(opacity_menu_);
  // menu->addMenu(other_menu_);
  menu->addAction("选项", [&]() { open_optionsettings(); });
  // menu->addMenu(help_menu_);
  menu->addAction("退出", [&]() { this->close(); });

  // 连接设置中的修改颜色

  // 设置退出时delete
  //  setAttribute(Qt::WA_DeleteOnClose, true);
  //  setAttribute(Qt::WA_DeleteOnClose);
  //  setAttribute(Qt::WA_TransparentForMouseEvents);
  //  setWindowOpacity(0.5);
}

void trafficmonitor::SentSpeedChanged() {
  std::lock_guard<std::mutex> lock(mutex_);
  double speed = networkport.get_sent_speed(port_num);
  if (network_unit == 0) {
    show_speed(speed, sent_speed, sent_unit);
  } else {
    speed *= 8;
    show_speed(speed, sent_speed, sent_unit);
  }
}
void trafficmonitor::ReceivedSpeedChanged() {
  std::lock_guard<std::mutex> lock(mutex_);
  double speed = networkport.get_received_speed(port_num);
  if (network_unit == 0) {
    show_speed(speed, received_speed, received_unit);
  } else {
    speed *= 8;
    show_speed(speed, received_speed, received_unit);
  }
}

void trafficmonitor::HoverWidgetShow() {
  if (hoverwidget->isHidden()) {
    if (is_show_mouse_tips) {
      hoverwidget->move(QCursor::pos().x(), QCursor::pos().y() + 30);
      hoverwidget->set_traffic();
      hoverwidget->set_source();
      hoverwidget->setWindowFlags(hoverwidget->windowFlags() |
                                  Qt::WindowStaysOnTopHint);
      hoverwidget->show();
      this->setFocus();
    }
  }
}

void trafficmonitor::enterEvent(QEvent *event) {
  hoverwidget->reset_timer();
  m_Position = QCursor::pos();
  event->accept();
}

void trafficmonitor::leaveEvent(QEvent *event) {
  hoverwidget->close_timer();
  event->accept();
}

void trafficmonitor::change_mainwindow_info(OptionInfo *info) {
  std::lock_guard<std::mutex> lock(mutex_);
  is_specify_color = info->ois_specify_color;
  up_color = info->up_color;
  down_color = info->down_color;
  CPU_color = info->CPU_color;
  memory_color = info->memory_color;
  mainwindow_color = info->omainwindow_color;
  mainwindow_font = info->omainwindow_font;
  up_text = info->up_text;
  down_text = info->down_text;
  CPU_text = info->CPU_text;
  memory_text = info->memory_text;
  is_concise_mode = info->ois_concise_mode;
  is_show_network_unit = info->ois_show_network_unit;
  unit_choose = info->ounit_choose;
  network_unit = info->onetwork_unit;
  is_show_mouse_tips = info->ois_show_mouse_tips;
  memory_show_mode = info->omemory_show_mode;
  allow_to_exceed = info->oallow_to_exceed;
  double_click_action = info->odouble_click_action;
  //
  if (is_showing_more_info) {
    switch (memory_show_mode) {
    case 0: {
      memory_unit->setText("%");
      break;
    }
    default:
      memory_unit->setText("GB");
      break;
    }
  }
  //
  is_show_percent_sign = info->ois_show_percent_sign;
  if (is_showing_more_info) {
    if (!is_show_percent_sign) {
      CPU_unit->setText("");
      if (memory_show_mode == 0) {
        memory_unit->setText("");
      }
    } else {
      CPU_unit->setText("%");
      if (memory_show_mode == 0) {
        memory_unit->setText("%");
      }
    }
  }
  //
  if (is_showing_more_info != info->ois_showing_more_info) {
    is_showing_more_info = info->ois_showing_more_info;
    lshow_more_info->setChecked(is_showing_more_info);
    if (is_showing_more_info) {
      show_more_info();
    } else {
      hide_more_info();
    }
  }
  //
  is_lock_window_pos = info->ois_lock_window_pos;
  lock_window_pos->setChecked(is_lock_window_pos);
  //
  if (info->ois_always_top != is_always_top) {
    change_over_always_top();
    always_top->setChecked(is_always_top);
  }
  // 交换位置
  if (is_exchange_up_down != info->ois_exchange_up_down) {
    is_exchange_up_down = info->ois_exchange_up_down;
    exchange_up_down();
  }
  // 修改颜色
  if (!is_specify_color) {
    set_label_color(sent_speed, mainwindow_color);
    set_label_color(received_speed, mainwindow_color);
    set_label_color(sent_unit, mainwindow_color);
    set_label_color(received_unit, mainwindow_color);
    set_label_color(download, mainwindow_color);
    set_label_color(upload, mainwindow_color);
    if (is_showing_more_info) {
      set_label_color(CPU, mainwindow_color);
      set_label_color(memory, mainwindow_color);
      set_label_color(CPU_mes, mainwindow_color);
      set_label_color(memory_mes, mainwindow_color);
      set_label_color(CPU_unit, mainwindow_color);
      set_label_color(memory_unit, mainwindow_color);
    }
  } else {
    set_label_color(sent_speed, up_color);
    set_label_color(received_speed, down_color);
    set_label_color(sent_unit, up_color);
    set_label_color(received_unit, down_color);
    set_label_color(download, down_color);
    set_label_color(upload, up_color);
    if (is_showing_more_info) {
      set_label_color(CPU, CPU_color);
      set_label_color(memory, memory_color);
      set_label_color(CPU_mes, CPU_color);
      set_label_color(memory_mes, memory_color);
      set_label_color(CPU_unit, CPU_color);
      set_label_color(memory_unit, memory_color);
    }
  }
  // 修改字体
  sent_speed->setFont(mainwindow_font);
  received_speed->setFont(mainwindow_font);
  sent_unit->setFont(mainwindow_font);
  received_unit->setFont(mainwindow_font);
  download->setFont(mainwindow_font);
  upload->setFont(mainwindow_font);
  if (is_showing_more_info) {
    CPU->setFont(mainwindow_font);
    memory->setFont(mainwindow_font);
    CPU_mes->setFont(mainwindow_font);
    memory_mes->setFont(mainwindow_font);
    CPU_unit->setFont(mainwindow_font);
    memory_unit->setFont(mainwindow_font);
  }
  // 修改文本
  download->setText(down_text);
  upload->setText(up_text);
  if (is_showing_more_info) {
    CPU->setText(CPU_text);
    memory->setText(memory_text);
  }
  delete info;
}

void trafficmonitor::MessageChanged() {
  SentSpeedChanged();
  ReceivedSpeedChanged();
  if (is_showing_more_info) {
    if (cpu_memory_task != nullptr) {
      if (cpu_memory_task->is_getting_CPU_memory()) {
        CPUUsedChanged();
        MemoryUesdChanged();
      }
    }
  }
}
void trafficmonitor::closeEvent(QCloseEvent *event) {
  write_json();
  if (hoverwidget != nullptr) {
    hoverwidget->close();
  }
  if (options != nullptr) {
    options->close();
  }
  if (connect_details != nullptr) {
    connect_details->close();
  }
  if (is_showing_more_info) {
    cpu_memory_task->stop_getting_CPU_memory();
  }
  delete cpu_memory_task;
}
void trafficmonitor::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    switch (double_click_action) {
    case 0: {
      connect_details = new ConnectDetails(
          connect_details, networkport.get_port_details(port_num));
      connect_details->setAttribute(Qt::WA_DeleteOnClose);
      connect_details->show();
      break;
    }
    case 1: {
      is_showing_more_info = !is_showing_more_info;
      lshow_more_info->setChecked(is_showing_more_info);
      if (is_showing_more_info) {
        show_more_info();
      } else {
        hide_more_info();
      }
      break;
    }
    case 2: {
      open_optionsettings();
      break;
    }
    case 3: {
      std::thread taskmgr_task([]() { system("taskmgr"); });
      taskmgr_task.detach();
      break;
    }
    default: {
      break;
    }
    }
  }
}

void trafficmonitor::open_optionsettings() {
  auto _option_info = new OptionInfo;
  _option_info->omainwindow_color = mainwindow_color;
  _option_info->omainwindow_font = mainwindow_font;
  _option_info->ois_specify_color = is_specify_color;
  _option_info->up_color = up_color;
  _option_info->down_color = down_color;
  _option_info->CPU_color = CPU_color;
  _option_info->memory_color = memory_color;
  _option_info->up_text = up_text;
  _option_info->down_text = down_text;
  _option_info->CPU_text = CPU_text;
  _option_info->memory_text = memory_text;
  _option_info->ois_exchange_up_down = is_exchange_up_down;
  _option_info->ois_concise_mode = is_concise_mode;
  _option_info->ois_show_network_unit = is_show_network_unit;
  _option_info->ounit_choose = unit_choose;
  _option_info->onetwork_unit = network_unit;
  _option_info->ois_show_percent_sign = is_show_percent_sign;
  _option_info->ois_show_mouse_tips = is_show_mouse_tips;
  _option_info->ois_always_top = is_always_top;
  _option_info->ois_showing_more_info = is_showing_more_info;
  _option_info->ois_lock_window_pos = is_lock_window_pos;
  _option_info->omemory_show_mode = memory_show_mode;
  _option_info->oallow_to_exceed = allow_to_exceed;
  _option_info->odouble_click_action = double_click_action;

  options = new OptionSettings(options, _option_info);
  options->setAttribute(Qt::WA_DeleteOnClose);
  connect(options, &OptionSettings::send_mainwindow_info, this,
          &trafficmonitor::change_mainwindow_info);
  options->show();
}
void trafficmonitor::show_speed(double speed, QLabel *speed_label,
                                QLabel *unit_label) {
  if (unit_choose == 0) {
    if (speed >= 1024) {
      if (is_concise_mode) {
        unit_label->setText(QString("M/S"));
        speed_label->setText(QString::number(speed / 1024, 'f', 1));
      } else {
        if (network_unit == 0) {
          unit_label->setText(QString("MB/S"));
        } else {
          unit_label->setText(QString("Mb/S"));
        }
        speed_label->setText(QString::number(speed / 1024, 'f', 2));
      }
    } else {
      if (is_concise_mode) {
        unit_label->setText(QString("K/S"));
        speed_label->setText(QString::number(speed, 'f', 1));
      } else {
        if (network_unit == 0) {
          unit_label->setText(QString("KB/S"));
        } else {
          unit_label->setText(QString("Kb/S"));
        }
        speed_label->setText(QString::number(speed, 'f', 2));
      }
    }
  } else if (unit_choose == 1) {
    if (!is_show_network_unit) {
      if (is_concise_mode) {
        unit_label->setText(QString("K/S"));
        speed_label->setText(QString::number(speed, 'f', 1));
      } else {
        if (network_unit == 0) {
          unit_label->setText(QString("KB/S"));
        } else {
          unit_label->setText(QString("Kb/S"));
        }
        speed_label->setText(QString::number(speed, 'f', 2));
      }
    } else {
      if (is_concise_mode) {
        unit_label->setText(QString(""));
        speed_label->setText(QString::number(speed, 'f', 1));
      } else {
        unit_label->setText(QString(""));
        speed_label->setText(QString::number(speed, 'f', 2));
      }
    }
  } else {
    if (!is_show_network_unit) {
      if (is_concise_mode) {
        unit_label->setText(QString("M/S"));
        speed_label->setText(QString::number(speed / 1024, 'f', 1));
      } else {
        if (network_unit == 0) {
          unit_label->setText(QString("MB/S"));
        } else {
          unit_label->setText(QString("Mb/S"));
        }
        speed_label->setText(QString::number(speed / 1024, 'f', 2));
      }
    } else {
      if (is_concise_mode) {
        unit_label->setText(QString(""));
        speed_label->setText(QString::number(speed / 1024, 'f', 1));
      } else {
        unit_label->setText(QString(""));
        speed_label->setText(QString::number(speed / 1024, 'f', 2));
      }
    }
  }
}

void trafficmonitor::CPUUsedChanged() {
  double cpu_used = cpu_memory_task->tget_CPU_used();
  CPU_mes->setText(QString::number(cpu_used, 'f', 2));
}
void trafficmonitor::MemoryUesdChanged() {
  switch (memory_show_mode) {
  case 0: {
    double memory_percentage = cpu_memory_task->tget_memory_percentage();
    memory_mes->setText(QString::number(memory_percentage * 100, 'f', 2));
    break;
  }
  case 1: {
    double memory_used = cpu_memory_task->tget_memory_used();
    memory_mes->setText(QString::number(memory_used, 'f', 2));
    break;
  }
  case 2: {
    double memory_not_used = cpu_memory_task->tget_memory_not_used();
    memory_mes->setText(QString::number(memory_not_used, 'f', 2));
    break;
  }
  default:
    break;
  }
}
void trafficmonitor::read_json() {
  QFile file("mySettings.json");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject obj = doc.object();
    is_always_top = obj["is_always_top"].toBool();
    is_showing_more_info = obj["is_showing_more_info"].toBool();
    is_lock_window_pos = obj["is_lock_window_pos"].toBool();
    is_mainwindow_show = obj["is_mainwindow_show"].toBool();
    upellucidity = obj["upellucidity"].toDouble();
    is_exchange_up_down = obj["is_exchange_up_down"].toBool();
    is_concise_mode = obj["is_concise_mode"].toBool();
    is_show_mouse_tips = obj["is_show_mouse_tips"].toBool();
    memory_show_mode = obj["memory_show_mode"].toInt();
    network_unit = obj["network_unit"].toInt();
    unit_choose = obj["unit_choose"].toInt();
    is_show_network_unit = obj["is_show_network_unit"].toBool();
    is_show_percent_sign = obj["is_show_percent_sign"].toBool();
    allow_to_exceed = obj["allow_to_exceed"].toBool();
    double_click_action = obj["double_click_action"].toInt();
    // 读取主窗口文字颜色
    QJsonArray mainwindow_color_array = obj["mainwindow_color"].toArray();
    make_color(mainwindow_color, mainwindow_color_array);
    // 读取主窗口字体
    QJsonObject mainwindow_font_obj = obj["mainwindow_font"].toObject();
    QString family = mainwindow_font_obj["family"].toString();
    int pointSize = mainwindow_font_obj["pointSize"].toInt();
    bool bold = mainwindow_font_obj["bold"].toBool();
    bool italic = mainwindow_font_obj["italic"].toBool();
    mainwindow_font = QFont(family, pointSize);
    mainwindow_font.setBold(bold);
    mainwindow_font.setItalic(italic);
    //
    is_specify_color = obj["is_specify_color"].toBool();
    //
    QJsonObject specified_color_obj = obj["specified_color"].toObject();
    QJsonArray up_color_array = specified_color_obj["up_color"].toArray();
    QJsonArray down_color_array = specified_color_obj["down_color"].toArray();
    QJsonArray CPU_color_array = specified_color_obj["CPU_color"].toArray();
    QJsonArray memory_color_array =
        specified_color_obj["memory_color"].toArray();
    make_color(up_color, up_color_array);
    make_color(down_color, down_color_array);
    make_color(CPU_color, CPU_color_array);
    make_color(memory_color, memory_color_array);
    //
    QJsonObject mainwindow_text_obj = obj["mainwindow_text"].toObject();
    up_text = mainwindow_text_obj["up_text"].toString();
    down_text = mainwindow_text_obj["down_text"].toString();
    CPU_text = mainwindow_text_obj["CPU_text"].toString();
    memory_text = mainwindow_text_obj["memory_text"].toString();

    file.close();
  }
}
void trafficmonitor::write_json() {
  // 创建一个QJsonObject对象
  QJsonObject json;

  // 向QJsonObject对象中添加键值对，键为QString类型，值为QJsonValue类型
  json.insert("is_always_top", QJsonValue(is_always_top));
  json.insert("is_showing_more_info", QJsonValue(is_showing_more_info));
  json.insert("is_lock_window_pos", QJsonValue(is_lock_window_pos));
  json.insert("is_mainwindow_show", QJsonValue(is_mainwindow_show));
  json.insert("upellucidity", QJsonValue(upellucidity));
  json.insert("is_exchange_up_down", QJsonValue(is_exchange_up_down));
  json.insert("is_concise_mode", QJsonValue(is_concise_mode));
  json.insert("is_show_mouse_tips", QJsonValue(is_show_mouse_tips));
  json.insert("memory_show_mode", QJsonValue(memory_show_mode));
  json.insert("network_unit", QJsonValue(network_unit));
  json.insert("unit_choose", QJsonValue(unit_choose));
  json.insert("is_show_network_unit", QJsonValue(is_show_network_unit));
  json.insert("is_show_percent_sign", QJsonValue(is_show_percent_sign));
  json.insert("allow_to_exceed", QJsonValue(allow_to_exceed));
  json.insert("double_click_action", QJsonValue(double_click_action));
  // 主窗口文字颜色
  json.insert("mainwindow_color", save_color(mainwindow_color));
  // 主窗口字体
  QJsonObject mainwindow_font_obj;
  mainwindow_font_obj.insert("family", mainwindow_font.family());
  mainwindow_font_obj.insert("pointSize",
                             QJsonValue(mainwindow_font.pointSize()));
  mainwindow_font_obj.insert("bold", QJsonValue(mainwindow_font.bold()));
  mainwindow_font_obj.insert("italic", QJsonValue(mainwindow_font.italic()));
  json.insert("mainwindow_font", mainwindow_font_obj);
  //
  json.insert("is_specify_color", QJsonValue(is_specify_color));
  //
  QJsonObject specified_color_obj;
  specified_color_obj.insert("up_color", save_color(up_color));
  specified_color_obj.insert("down_color", save_color(down_color));
  specified_color_obj.insert("CPU_color", save_color(CPU_color));
  specified_color_obj.insert("memory_color", save_color(memory_color));
  json.insert("specified_color", specified_color_obj);
  //
  QJsonObject mainwindow_text_obj;
  mainwindow_text_obj.insert("up_text", up_text);
  mainwindow_text_obj.insert("down_text", down_text);
  mainwindow_text_obj.insert("CPU_text", CPU_text);
  mainwindow_text_obj.insert("memory_text", memory_text);
  json.insert("mainwindow_text", mainwindow_text_obj);
  // 将QJsonObject对象保存到QJsonDocument中
  QJsonDocument doc(json);
  // 将QJsonDocument对象转换为字符串格式，保存到json文件中
  QFile file("mySettings.json");
  if (file.open(QIODevice::WriteOnly)) {
    file.write(doc.toJson());
    file.close();
  }
}
void trafficmonitor::set_port_num(int num) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (num == -1) {
    port_num = networkport.get_auto_port_num();
    return;
  }
  if (num <= networkport.get_port_name_size()) {
    port_num = num;
  }
}
void trafficmonitor::set_upellucidity(double upellucidity_) {
  std::lock_guard<std::mutex> lock(mutex_);
  upellucidity = upellucidity_;
  switch (int(upellucidity * 10)) {
  case 10: {
    this->setWindowOpacity(1.0);
    break;
  }
  case 8: {
    this->setWindowOpacity(0.8);
    break;
  }
  case 6: {
    this->setWindowOpacity(0.6);
    break;
  }
  case 4: {
    this->setWindowOpacity(0.4);
    break;
  }
  default:
    break;
  }
}
void trafficmonitor::set_fillet() {
  QBitmap bmp(this->size());
  // 创建的位图填充为白色
  bmp.fill();
  // 将图片添加为绘制对象
  QPainter painter(&bmp);
  // 不使用画笔的线条绘制
  painter.setPen(Qt::NoPen);
  // 使用黑色画刷进行绘制
  painter.setBrush(Qt::black);
  // 绘制圆角矩形，圆角半径40
  painter.drawRoundedRect(bmp.rect(), 8, 8);
  // 使用圆角矩形位图对窗户实现遮罩（即圆角窗口）
  this->setMask(bmp);
}
void trafficmonitor::show_more_info() {
  is_showing_more_info = true;
  if (cpu_memory_layout == nullptr) {
    make_more_info();
    if (cpu_memory_task == nullptr) {
      cpu_memory_task = new CPUMemoryTask;
    }
    cpu_memory_task->set_getting_CPU_memory();
  }
  mainwindow_layout->addLayout(cpu_memory_layout);
  set_fixed_size(this, 250, 70);
  set_fillet();
}
void trafficmonitor::hide_more_info() {
  is_showing_more_info = false;
  if (cpu_memory_layout != nullptr) {
    if (cpu_memory_task != nullptr) {
      cpu_memory_task->stop_getting_CPU_memory();
    }
    clear_layout(cpu_memory_layout);
    cpu_memory_layout = nullptr;
  }
  set_fixed_size(this, 250, 35);
  set_fillet();
}

void trafficmonitor::init_label(QLabel *&label, const QString &str,
                                QColor color) {
  label = new QLabel(str);
  if (is_specify_color) {
    set_label_color(label, color);
  } else {
    set_label_color(label, mainwindow_color);
  }
  label->setFont(mainwindow_font);
}

void trafficmonitor::make_more_info() {
  init_label(CPU, "CPU:", CPU_color);
  init_label(memory, "内存:", memory_color);
  init_label(CPU_mes, "0", CPU_color);
  init_label(memory_mes, "0.00", memory_color);
  if (is_show_percent_sign) {
    init_label(CPU_unit, "%", CPU_color);
    if (memory_show_mode == 0) {
      init_label(memory_unit, "%", memory_color);
    } else {
      init_label(memory_unit, "GB", memory_color);
    }
  } else {
    init_label(CPU_unit, "", CPU_color);
    if (memory_show_mode == 0) {
      init_label(memory_unit, "", memory_color);
    } else {
      init_label(memory_unit, "GB", memory_color);
    }
  }
  hlist3 = make_hwidgetlist(CPU, CPU_mes, CPU_unit);
  hlist4 = make_hwidgetlist(memory, memory_mes, memory_unit);
  hlist3->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  hlist4->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  cpu_memory_layout = new QHBoxLayout;
  cpu_memory_layout->addWidget(hlist3);
  cpu_memory_layout->addWidget(hlist4);
}
void trafficmonitor::exchange_up_down() {
  int index1 = up_down_layout->indexOf(hlist1);
  int index2 = up_down_layout->indexOf(hlist2);
  if (index1) {
    up_down_layout->takeAt(index2);
    up_down_layout->insertWidget(index2, hlist1);

    up_down_layout->takeAt(index1);
    up_down_layout->insertWidget(index1, hlist2);
  } else {
    up_down_layout->takeAt(index1);
    up_down_layout->insertWidget(index1, hlist2);

    up_down_layout->takeAt(index2);
    up_down_layout->insertWidget(index2, hlist1);
  }
}
void trafficmonitor::change_over_always_top() {
  is_always_top = !is_always_top;
  if (this->windowFlags() & Qt::WindowStaysOnTopHint) {
    // 如果有，则设置无
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
    this->show();
  } else {
    // 如果无，则设置有
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->show();
  }
}