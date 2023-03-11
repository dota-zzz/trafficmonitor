#include "optionsettings.h"
#include <QEvent>
#include <QFont>
#include <QFontDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <vector>

OptionSettings::OptionSettings(OptionSettings *&ptr, OptionInfo *_option_info,
                               QWidget *parent)
    : optionsettings_ptr(ptr) {
  option_info = _option_info;
  create_ui();
  create_logic();
}

void OptionSettings::create_ui() {
  setFixedSize(window_width, window_height);

  tab_widget = new QTabWidget;

  // 添加tab
  tab_widget->addTab(create_mwindowset(), "主窗口设置");
  // tab_widget->addTab(create_tbarwindowset(), "任务栏窗口设置");
  // tab_widget->addTab(create_generalset(), "常规设置");

  btn1 = new QPushButton("确定");
  btn2 = new QPushButton("取消");
  btn3 = new QPushButton("应用");
  auto hlayout_ = new QHBoxLayout;
  hlayout_->addStretch(1);
  hlayout_->addWidget(btn1);
  hlayout_->addWidget(btn2);
  hlayout_->addWidget(btn3);
  auto widget1_ = new QWidget;
  widget1_->setLayout(hlayout_);

  auto vlayout_ = new QVBoxLayout;
  vlayout_->addWidget(tab_widget, 1);
  vlayout_->addWidget(widget1_);

  setModal(true);
  setLayout(vlayout_);
}
void OptionSettings::create_logic() {
  m_f_cbox1->setChecked(option_info->ois_specify_color);
  m_s_cbox1->setChecked(option_info->ois_exchange_up_down);
  m_s_cbox2->setChecked(option_info->ois_concise_mode);
  if (option_info->onetwork_unit == 0) {
    m_u_rbtn1->setChecked(true);
  } else {
    m_u_rbtn2->setChecked(true);
  }
  m_u_hpair1->second->setCurrentIndex(option_info->ounit_choose);
  m_s_pair1->second->setCurrentIndex(option_info->omemory_show_mode);
  m_o_hpair1->second->setCurrentIndex(option_info->odouble_click_action);
  m_u_cbox1->setEnabled(option_info->ounit_choose);
  m_u_cbox1->setChecked(m_u_cbox1->isEnabled() &&
                        option_info->ois_show_network_unit);
  m_u_cbox2->setChecked(!(option_info->ois_show_percent_sign));
  m_s_cbox4->setChecked(option_info->ois_show_mouse_tips);
  m_o_cbox1->setChecked(option_info->ois_always_top);
  m_o_cbox2->setChecked(option_info->ois_showing_more_info);
  m_o_cbox3->setChecked(option_info->ois_lock_window_pos);
  m_o_cbox4->setChecked(option_info->oallow_to_exceed);
  connect(btn1, &QPushButton::clicked, [&]() {
    auto option_info_ = new OptionInfo(*option_info);
    emit send_mainwindow_info(option_info_);
    close();
  });
  connect(btn2, &QPushButton::clicked, [&]() { close(); });
  connect(btn3, &QPushButton::clicked, [&]() {
    auto option_info_ = new OptionInfo(*option_info);
    emit send_mainwindow_info(option_info_);
  });
  connect(m_f_hpair1->second, &QPushButton::clicked, [&]() {
    bool ok;
    QFont f = QFontDialog::getFont(&ok);
    option_info->omainwindow_font = f;
    m_f_hpair2->second->setValue(f.pointSize());
    f.setPointSize(9);
    m_f_hpair1->second->setFont(f);
    m_f_hpair1->second->setText(f.family());
  });
  connect(m_f_hpair2->second, &QSpinBox::valueChanged, [&]() {
    option_info->omainwindow_font.setPointSize(m_f_hpair2->second->value());
  });
  connect(m_f_cbox1, &QCheckBox::toggled, [&](bool checked) {
    if (checked) {
      option_info->ois_specify_color = true;
      m_f_cb->set_specify_mode();
    } else {
      option_info->ois_specify_color = false;
      m_f_cb->set_common_mode();
    }
  });
  connect(m_s_btn1, &QPushButton::clicked, [&]() {
    TextSpecifier *t =
        new TextSpecifier(std::vector<QString>{"上传", "下载", "CPU", "内存"},
                          std::vector<QString *>{&(option_info->up_text),
                                                 &(option_info->down_text),
                                                 &(option_info->CPU_text),
                                                 &(option_info->memory_text)});
    t->show();
  });
  connect(m_s_cbox1, &QCheckBox::toggled,
          [&](bool checked) { option_info->ois_exchange_up_down = checked; });
  connect(m_s_cbox2, &QCheckBox::toggled,
          [&](bool checked) { option_info->ois_concise_mode = checked; });
  connect(m_u_rbtn1, &QRadioButton::toggled, [&](bool checked) {
    if (checked) {
      option_info->onetwork_unit = 0;
    } else {
      option_info->onetwork_unit = 1;
    }
  });
  connect(m_u_hpair1->second, &QComboBox::currentIndexChanged, [&](int index) {
    option_info->ounit_choose = index;
    if (index == 0) {
      m_u_cbox1->setChecked(false);
      m_u_cbox1->setEnabled(false);
      option_info->ois_show_network_unit = false;
    } else {
      m_u_cbox1->setEnabled(true);
    }
  });
  connect(m_u_cbox1, &QCheckBox::toggled,
          [&](bool checked) { option_info->ois_show_network_unit = checked; });
  connect(m_u_cbox2, &QCheckBox::toggled,
          [&](bool checked) { option_info->ois_show_percent_sign = !checked; });
  connect(m_s_cbox4, &QCheckBox::toggled,
          [&](bool checked) { option_info->ois_show_mouse_tips = checked; });
  connect(m_o_cbox1, &QCheckBox::toggled,
          [&](bool checked) { option_info->ois_always_top = checked; });
  connect(m_o_cbox2, &QCheckBox::toggled,
          [&](bool checked) { option_info->ois_showing_more_info = checked; });
  connect(m_o_cbox3, &QCheckBox::toggled,
          [&](bool checked) { option_info->ois_lock_window_pos = checked; });
  connect(m_s_pair1->second, &QComboBox::currentIndexChanged,
          [&](int index) { option_info->omemory_show_mode = index; });
  connect(m_o_cbox4, &QCheckBox::toggled,
          [&](bool checked) { option_info->oallow_to_exceed = checked; });
  connect(m_o_hpair1->second, &QComboBox::currentIndexChanged,
          [&](int index) { option_info->odouble_click_action = index; });
}
// 主窗口设置
QWidget *OptionSettings::create_mwindowset() {
  auto widget1_ = new QWidget;
  auto widget_ = new QWidget(widget1_);
  auto v_layout_ = new QVBoxLayout;
  // 添加groupbox
  v_layout_->addWidget(create_mwindowset_fontcolor());
  v_layout_->addWidget(create_mwindowset_showtext());
  v_layout_->addWidget(create_unitset(m_u_rbtn1, m_u_rbtn2, m_u_bgroup,
                                      m_u_hpair1, m_u_cbox1, m_u_cbox2));
  v_layout_->addWidget(create_mwindowset_other());
  widget_->setLayout(v_layout_);
  widget_->setMinimumWidth(window_width - 50);
  return widget1_;
}
// 主窗口设置：字体和颜色
QWidget *OptionSettings::create_mwindowset_fontcolor() {
  m_f_hpair1 = new HlayoutPair<QLabel, QPushButton>;
  m_f_hpair1->first->setText("字体：");
  QFont _f = option_info->omainwindow_font;
  _f.setPointSize(9);
  m_f_hpair1->second->setFont(_f);
  m_f_hpair1->second->setText(_f.family());
  m_f_hpair2 = new HlayoutPair<QLabel, MySpinBox>;
  m_f_hpair2->first->setText("字体大小：");
  m_f_hpair2->second->setMinimum(5);
  m_f_hpair2->second->setMaximum(72);
  m_f_hpair2->second->setValue(option_info->omainwindow_font.pointSize());
  QLabel *m_f_l = new QLabel("字体颜色：");
  m_f_cb = new SpecifyButton(
      &(option_info->omainwindow_color), option_info->ois_specify_color,
      std::vector<QString>{option_info->up_text, option_info->down_text,
                           option_info->CPU_text, option_info->memory_text},
      std::vector<QColor *>{
          &(option_info->up_color), &(option_info->down_color),
          &(option_info->CPU_color), &(option_info->memory_color)});
  m_f_hpair3 = make_hwidgetlist(m_f_l, m_f_cb);
  m_f_cbox1 = new QCheckBox("指定每个项目的颜色");

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(m_f_hpair1, 0, 0);
  g_layout_->addWidget(m_f_hpair2, 0, 1);
  g_layout_->addWidget(m_f_hpair3, 1, 0);
  g_layout_->addWidget(m_f_cbox1, 1, 1);

  auto g_widget_ = new QGroupBox("颜色和字体");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 主窗口设置：显示文本
QWidget *OptionSettings::create_mwindowset_showtext() {
  m_s_btn1 = new QPushButton("显示文本设置(T)...");
  m_s_cbox1 = new QCheckBox("交换上传和下载的位置");
  m_s_cbox2 = new QCheckBox("网速显示简介模式");
  // m_s_cbox3 = new QCheckBox("数值和单位用空格分隔");
  m_s_cbox4 = new QCheckBox("显示鼠标提示");
  m_s_pair1 = new HlayoutPair<QLabel, MyComboBox>;
  m_s_pair1->first->setText("内存显示方式：");
  m_s_pair1->second->addItem("已使用百分比", 1);
  m_s_pair1->second->addItem("内存已使用", 2);
  m_s_pair1->second->addItem("内存可用", 3);
  // connect(m_s_pair1->second, &QAbstractItemView::wheelEvent,
  // &emptyWheelEvent); m_s_pair1->second->setWrapping(false);
  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(m_s_btn1, 0, 0);
  g_layout_->addWidget(m_s_cbox1, 1, 0);
  g_layout_->addWidget(m_s_cbox2, 1, 1);
  // g_layout_->addWidget(m_s_cbox3, 2, 0);
  g_layout_->addWidget(m_s_cbox4, 2, 0);
  g_layout_->addWidget(m_s_pair1, 3, 0);

  auto g_widget_ = new QGroupBox("显示文本");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 共用函数：单位设置
QWidget *OptionSettings::create_unitset(QRadioButton *&rbtn1,
                                        QRadioButton *&rbtn2,
                                        QButtonGroup *&btngroup,
                                        HlayoutPair<QLabel, MyComboBox> *&hpair,
                                        QCheckBox *&cbox1, QCheckBox *&cbox2) {
  rbtn1 = new QRadioButton("B(字节)");
  rbtn2 = new QRadioButton("b(比特)");
  auto label_ = new QLabel("网速单位：");
  auto hwidgetlist_ = make_hwidgetlist(label_, rbtn1, rbtn2);

  hpair = new HlayoutPair<QLabel, MyComboBox>;
  hpair->first->setText("单位选择：");
  hpair->second->addItem("自动", 1);
  hpair->second->addItem("固定为KB/s", 2);
  hpair->second->addItem("固定为MB/s", 3);
  cbox1 = new QCheckBox("不显示网速单位");
  cbox2 = new QCheckBox("不显示百分号");

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(hwidgetlist_, 0, 0);
  g_layout_->addWidget(hpair, 1, 0);
  g_layout_->addWidget(cbox1, 2, 0);
  g_layout_->addWidget(cbox2, 2, 1);

  auto g_widget_ = new QGroupBox("单位设置");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 主窗口设置 其他
QWidget *OptionSettings::create_mwindowset_other() {
  m_o_cbox1 = new QCheckBox("总是置顶");
  m_o_cbox2 = new QCheckBox("显示更多信息");
  m_o_cbox3 = new QCheckBox("锁定窗口位置");
  m_o_cbox4 = new QCheckBox("允许超出屏幕边界");
  // m_o_cbox5 = new QCheckBox("有程序全屏运行时隐藏主窗口");
  m_o_hpair1 = new HlayoutPair<QLabel, MyComboBox>;
  m_o_hpair1->first->setText("双击动作：");
  m_o_hpair1->second->addItem("打开连接详情");
  // m_o_hpair1->second->addItem("打开历史流量统计");
  m_o_hpair1->second->addItem("显示/隐藏更多信息");
  m_o_hpair1->second->addItem("打开选项设置");
  m_o_hpair1->second->addItem("打开任务管理器");
  // m_o_hpair1->second->addItem("打开指定应用程序");
  // m_o_hpair1->second->addItem("更换皮肤");
  m_o_hpair1->second->addItem("无");

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(m_o_cbox1, 0, 0);
  g_layout_->addWidget(m_o_cbox2, 0, 1);
  g_layout_->addWidget(m_o_cbox3, 1, 0);
  g_layout_->addWidget(m_o_cbox4, 1, 1);
  // g_layout_->addWidget(m_o_cbox5, 2, 0);
  g_layout_->addWidget(m_o_hpair1, 2, 0);

  auto g_widget_ = new QGroupBox("其他");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 任务栏窗口设置 颜色和字体
QWidget *OptionSettings::create_tbarwindowset_fontcolor() {
  t_f_hpair1 = new HlayoutPair<QLabel, QPushButton>;
  t_f_hpair1->first->setText("字体：");
  t_f_hpair2 = new HlayoutPair<QLabel, MySpinBox>;
  t_f_hpair2->first->setText("字体大小：");
  t_f_hpair2->second->setMinimum(5);
  t_f_hpair2->second->setMaximum(72);
  t_f_hpair3 = new HlayoutPair<QLabel, QPushButton>;
  t_f_hpair3->first->setText("文本颜色");
  t_f_cbox1 = new QCheckBox("指定每个项目的颜色");
  t_f_hpair4 = new HlayoutPair<QLabel, QPushButton>("背景颜色");
  t_f_cbox2 = new QCheckBox("背景透明");
  t_f_cbox3 = new QCheckBox("根据任务栏颜色自动设置背景色");

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(t_f_hpair1, 0, 0);
  g_layout_->addWidget(t_f_hpair2, 0, 1);
  g_layout_->addWidget(t_f_hpair3, 1, 0);
  g_layout_->addWidget(t_f_cbox1, 1, 1);
  g_layout_->addWidget(t_f_hpair4, 2, 0);
  g_layout_->addWidget(t_f_cbox2, 2, 1);
  g_layout_->addWidget(t_f_cbox3, 3, 0);

  auto g_widget_ = new QGroupBox("颜色和字体");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 任务栏窗口设置
QWidget *OptionSettings::create_tbarwindowset() {
  t_widget = new QWidget;
  t_m_widget = new QWidget(t_widget);
  auto v_layout_ = new QVBoxLayout;

  // 添加groupbox
  v_layout_->addWidget(create_tbarwindowset_fontcolor());
  v_layout_->addWidget(create_tbarwindowset_displayset());
  v_layout_->addWidget(create_unitset(t_u_rbtn1, t_u_rbtn2, t_u_bgroup,
                                      t_u_hpair1, t_u_cbox1, t_u_cbox2));
  v_layout_->addWidget(create_tbarwindowset_tbarwindowset());
  v_layout_->addWidget(create_tbarwindowset_sourceuchart());

  t_m_widget->setLayout(v_layout_);
  t_m_widget->setMinimumWidth(window_width - 50);
  get_scrollbar(t_scrollbar, t_widget, t_m_widget);
  t_widget->installEventFilter(this);
  // auto scrollBar_ = new QScrollBar(Qt::Vertical);
  // auto widget1_ = new QWidget;
  // auto hlayout_ = new QHBoxLayout;
  // hlayout_->addWidget(widget_);
  // hlayout_->addWidget(scrollBar_);
  // widget1_->setLayout(hlayout_);
  // widget1_->setMinimumHeight(1200);
  return t_widget;
}
// 任务栏窗口设置 显示设置
QWidget *OptionSettings::create_tbarwindowset_displayset() {
  t_d_btn1 = new QPushButton("显示文本设置(T)...");
  t_d_btn2 = new QPushButton("显示设置(O)...");
  t_d_cbox1 = new QCheckBox("网速显示简洁模式");
  t_d_cbox2 = new QCheckBox("显示鼠标提示");
  t_d_cbox3 = new QCheckBox("数值右对齐");
  t_d_cbox4 = new QCheckBox("水平排列");
  t_d_cbox5 = new QCheckBox("数值和单位用空格分隔");

  auto label1_1_ = new QLabel("网速数据宽度：");
  auto label1_2_ = new QLabel("字符");
  t_d_cbobox1 = new MyComboBox;
  t_d_cbobox1->addItem("3");
  t_d_cbobox1->addItem("4");
  t_d_cbobox1->addItem("5");
  t_d_cbobox1->addItem("6");
  t_d_cbobox1->addItem("7");
  auto hlist1_ = make_hwidgetlist(label1_1_, t_d_cbobox1, label1_2_);

  auto label2_1 = new QLabel("内存显示方式：");
  t_d_cbobox2 = new MyComboBox;
  t_d_cbobox2->addItem("已使用百分比");
  t_d_cbobox2->addItem("内存可使用");
  t_d_cbobox2->addItem("内存可用");
  auto hlist2_ = make_hwidgetlist(label2_1, t_d_cbobox2);

  auto label3_1 = new QLabel("项目间距");
  t_d_sbox1 = new MySpinBox;
  t_d_sbox1->setMinimum(0);
  t_d_sbox1->setMaximum(32);
  auto hlist3_ = make_hwidgetlist(label3_1, t_d_sbox1);

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(t_d_btn1, 0, 0);
  g_layout_->addWidget(t_d_btn2, 0, 1);
  g_layout_->addWidget(t_d_cbox1, 1, 0);
  g_layout_->addWidget(t_d_cbox2, 1, 1);
  g_layout_->addWidget(t_d_cbox3, 2, 0);
  g_layout_->addWidget(t_d_cbox4, 2, 1);
  g_layout_->addWidget(t_d_cbox5, 3, 0);
  g_layout_->addWidget(hlist1_, 4, 0);
  g_layout_->addWidget(hlist2_, 5, 0);
  g_layout_->addWidget(hlist3_, 6, 0);

  auto g_widget_ = new QGroupBox("显示设置");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 任务栏窗口设置 任务栏窗口
QWidget *OptionSettings::create_tbarwindowset_tbarwindowset() {
  t_t_cbox1 = new QCheckBox("任务栏窗口显示在任务栏的左侧");
  t_t_cbox2 =
      new QCheckBox("任务栏窗口靠近图标而不是靠近任务栏的两侧(仅Windows11)");

  auto label1_1_ = new QLabel("双击动作：");
  t_t_cbobox1 = new MyComboBox;
  t_t_cbobox1->addItem("打开连接详情");
  t_t_cbobox1->addItem("打开历史流量统计");
  t_t_cbobox1->addItem("显示/隐藏CPU和内存占用率");
  t_t_cbobox1->addItem("打开选项设置");
  t_t_cbobox1->addItem("打开任务管理器");
  t_t_cbobox1->addItem("打开指定应用程序");
  t_t_cbobox1->addItem("无");
  auto hlist_ = make_hwidgetlist(label1_1_, t_t_cbobox1);

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(t_t_cbox1, 0, 0);
  g_layout_->addWidget(t_t_cbox2, 1, 0);
  g_layout_->addWidget(hlist_, 2, 0);

  auto g_widget_ = new QGroupBox("任务栏窗口");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 任务栏窗口设置 资源占用图
QWidget *OptionSettings::create_tbarwindowset_sourceuchart() {
  t_r_cbox1 = new QCheckBox("显示资源占用图");
  t_r_cbox2 = new QCheckBox("显示虚线框");
  t_r_cbox3 = new QCheckBox("显示网速占用图");

  auto label1_1_ = new QLabel("网速占用图显示最大值：");
  t_r_sbox1 = new MySpinBox;
  t_r_sbox1->setMinimum(512);
  t_r_sbox1->setMaximum(512);
  t_r_cbobox1 = new MyComboBox;
  t_r_cbobox1->addItem("KB");
  auto hlist1_ = make_hwidgetlist(label1_1_, t_r_sbox1, t_r_cbobox1);

  auto label2_1_ = new QLabel("占用图颜色：");
  t_r_btn1 = new QPushButton;
  auto hlist2_ = make_hwidgetlist(label2_1_, t_r_btn1);

  t_r_rbtn1 = new QRadioButton("柱状图");
  t_r_rbtn2 = new QRadioButton("横向滚动图");
  auto hlist3_ = make_hwidgetlist_flabel("占用图类型：", t_r_rbtn1, t_r_rbtn2);

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(t_r_cbox1, 0, 0);
  g_layout_->addWidget(t_r_cbox2, 0, 1);
  g_layout_->addWidget(t_r_cbox3, 1, 0);
  g_layout_->addWidget(hlist1_, 2, 0);
  g_layout_->addWidget(hlist2_, 3, 0);
  g_layout_->addWidget(hlist3_, 4, 0);

  auto g_widget_ = new QGroupBox("资源占用图");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 常规设置 应用程序设置
QWidget *OptionSettings::create_generalset_appset() {
  g_a_cbox1 = new QCheckBox("启动时检查更新");
  g_a_btn1 = new QPushButton("立即检查(C)");
  auto hlist1_ = make_hwidgetlist(g_a_cbox1, g_a_btn1);

  g_a_rbtn1 = new QRadioButton("GitHub");
  g_a_rbtn2 = new QRadioButton("Gitee");
  auto hlist2_ = make_hwidgetlist_flabel("更新源：", g_a_rbtn1, g_a_rbtn2);

  g_a_cbox2 = new QCheckBox("开机时自动运行");
  g_a_btn2 = new QPushButton("重新设置开机自动运行");

  g_a_cbobox1 = new MyComboBox;
  g_a_cbobox1->addItem("跟随系统");
  g_a_cbobox1->addItem("English");
  g_a_cbobox1->addItem("简体中文");
  g_a_cbobox1->addItem("繁體中文");
  auto hlist3_ = make_hwidgetlist_flabel("语言：", g_a_cbobox1);

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(hlist1_, 0, 0);
  g_layout_->addWidget(hlist2_, 1, 0);
  g_layout_->addWidget(g_a_cbox2, 2, 0);
  g_layout_->addWidget(g_a_btn2, 3, 0);
  g_layout_->addWidget(hlist3_, 4, 0);

  auto g_widget_ = new QGroupBox("应用程序设置");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 常规设置
QWidget *OptionSettings::create_generalset() {
  g_widget = new QWidget;
  g_m_widget = new QWidget(g_widget);
  auto v_layout_ = new QVBoxLayout;
  // 添加groupbox
  v_layout_->addWidget(create_generalset_appset());
  v_layout_->addWidget(create_generalset_configdata());
  v_layout_->addWidget(create_generalset_inform());
  v_layout_->addWidget(create_generalset_hwaremonitor());
  v_layout_->addWidget(create_generalset_seniorset());
  v_layout_->addWidget(create_generalset_display());

  g_m_widget->setLayout(v_layout_);
  g_m_widget->setMinimumWidth(window_width - 50);
  get_scrollbar(g_scrollbar, g_widget, g_m_widget);
  g_widget->installEventFilter(this);
  return g_widget;
}
// 常规设置 配置和数据文件
QWidget *OptionSettings::create_generalset_configdata() {
  g_c_rbtn1 = new QRadioButton("保存到Appdata目录");
  g_c_rbtn2 = new QRadioButton("保存到程序所在目录");
  auto hlist1_ = make_hwidgetlist(g_c_rbtn1, g_c_rbtn2);
  g_c_btn1 = new QPushButton("打开配置文件所在目录(D)");

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(hlist1_, 0, 0);
  g_layout_->addWidget(g_c_btn1, 1, 0);

  auto g_widget_ = new QGroupBox("配置和数据文件");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 常规设置 通知消息
QWidget *OptionSettings::create_generalset_inform() {
  g_i_cbox1 = new QCheckBox("今日已使用流量达到");
  g_i_cbox2 = new QCheckBox("内存利用率达到");
  g_i_cbox3 = new QCheckBox("CPU温度达到");
  g_i_cbox4 = new QCheckBox("显卡温度达到");
  g_i_cbox5 = new QCheckBox("硬盘温度达到");
  g_i_cbox6 = new QCheckBox("主板温度达到");
  g_i_sbox1 = new MySpinBox;
  g_i_sbox2 = new MySpinBox;
  g_i_sbox3 = new MySpinBox;
  g_i_sbox4 = new MySpinBox;
  g_i_sbox5 = new MySpinBox;
  g_i_sbox6 = new MySpinBox;
  g_i_sbox1->setMaximum(100);
  g_i_sbox1->setMinimum(0);
  g_i_sbox2->setMaximum(100);
  g_i_sbox2->setMinimum(0);
  g_i_sbox3->setMaximum(100);
  g_i_sbox3->setMinimum(0);
  g_i_sbox4->setMaximum(100);
  g_i_sbox4->setMinimum(0);
  g_i_sbox5->setMaximum(100);
  g_i_sbox5->setMinimum(0);
  g_i_sbox6->setMaximum(100);
  g_i_sbox6->setMinimum(0);
  g_i_cbobox1 = new MyComboBox;
  g_i_cbobox1->addItem("MB");
  g_i_cbobox1->addItem("KB");
  auto label1_1 = new QLabel("时通知");
  auto label2_1 = new QLabel("%时通知");
  auto label3_1 = new QLabel("℃时通知");
  auto label4_1 = new QLabel("℃时通知");
  auto label5_1 = new QLabel("℃时通知");
  auto label6_1 = new QLabel("℃时通知");

  auto hlist1_ = make_hwidgetlist(g_i_cbox1, g_i_sbox1, g_i_cbobox1, label1_1);
  auto hlist2_ = make_hwidgetlist(g_i_cbox2, g_i_sbox2, label2_1);
  auto hlist3_ = make_hwidgetlist(g_i_cbox3, g_i_sbox3, label3_1);
  auto hlist4_ = make_hwidgetlist(g_i_cbox4, g_i_sbox4, label4_1);
  auto hlist5_ = make_hwidgetlist(g_i_cbox5, g_i_sbox5, label5_1);
  auto hlist6_ = make_hwidgetlist(g_i_cbox6, g_i_sbox6, label6_1);

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(hlist1_, 0, 0);
  g_layout_->addWidget(hlist2_, 1, 0);
  g_layout_->addWidget(hlist3_, 2, 0);
  g_layout_->addWidget(hlist4_, 3, 0);
  g_layout_->addWidget(hlist5_, 4, 0);
  g_layout_->addWidget(hlist6_, 5, 0);

  auto g_widget_ = new QGroupBox("通知消息");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 常规设置 硬件监控
QWidget *OptionSettings::create_generalset_hwaremonitor() {
  g_h_cbox1 = new QCheckBox("CPU");
  g_h_cbox2 = new QCheckBox("显卡");
  g_h_cbox3 = new QCheckBox("硬盘");
  g_h_cbox4 = new QCheckBox("主板");
  auto hlist1_ = make_hwidgetlist(g_h_cbox1, g_h_cbox2, g_h_cbox3, g_h_cbox4);

  g_h_sbox1 = new MyComboBox;
  auto hlist2_ = make_hwidgetlist_flabel("选择监控的硬盘：", g_h_sbox1);

  g_h_sbox2 = new MyComboBox;
  auto hlist3_ = make_hwidgetlist_flabel("选择监控的硬盘：", g_h_sbox2);

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(hlist1_, 0, 0);
  g_layout_->addWidget(hlist2_, 1, 0);
  g_layout_->addWidget(hlist3_, 2, 0);

  auto g_widget_ = new QGroupBox("硬件监控");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 常规设置 高级
QWidget *OptionSettings::create_generalset_seniorset() {
  g_ad_cbox1 = new QCheckBox("显示所有网络连接");
  g_ad_btn1 = new QPushButton("选择要监控的网络连接(S)...");

  g_ad_rbtn1 = new QRadioButton("基于CPU使用事件");
  g_ad_rbtn2 = new QRadioButton("使用性能计数器");
  auto hlist1_ =
      make_hwidgetlist_flabel("CPU使用率获取方式", g_ad_rbtn1, g_ad_rbtn2);

  g_ad_sbox1 = new MySpinBox;
  g_ad_sbox1->setMinimum(1000);
  g_ad_sbox1->setMaximum(2000);
  g_ad_btn2 = new QPushButton("恢复默认(R)");
  auto label1_1 = new QLabel("毫秒");
  auto hlist2_ = make_hwidgetlist_flabel("监控时间间隔：", g_ad_sbox1, label1_1,
                                         g_ad_btn2);

  g_ad_btn3 = new QPushButton("插件管理(P)...");

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(g_ad_cbox1, 0, 0);
  g_layout_->addWidget(g_ad_btn1, 1, 0);
  g_layout_->addWidget(hlist1_, 2, 0);
  g_layout_->addWidget(hlist2_, 3, 0);
  g_layout_->addWidget(g_ad_btn3, 4, 0);

  auto g_widget_ = new QGroupBox("硬件监控");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}
// 常规设置 显示
QWidget *OptionSettings::create_generalset_display() {
  g_d_cbox1 = new QCheckBox("显示通知区域");
  g_d_cbox2 = new QCheckBox("允许皮肤覆盖字体设置");
  g_d_cbox3 = new QCheckBox("允许皮肤覆盖显示文本设置");

  auto g_layout_ = new QGridLayout;
  g_layout_->addWidget(g_ad_cbox1, 0, 0);
  g_layout_->addWidget(g_d_cbox2, 0, 1);
  g_layout_->addWidget(g_d_cbox3, 1, 0);

  auto g_widget_ = new QGroupBox("显示");
  g_widget_->setLayout(g_layout_);
  return g_widget_;
}

// 共用函数 滚动条设置
void OptionSettings::get_scrollbar(QScrollBar *&scrollbar, QWidget *p_widget,
                                   QWidget *m_widget) {
  scrollbar = new QScrollBar(p_widget);
  scrollbar->installEventFilter(p_widget);
  scrollbar->setMaximum(15);
  scrollbar->setGeometry(window_width - 60, 3, 25, window_height - 130);
  connect(scrollbar, &QScrollBar::valueChanged,
          [=](int value) { m_widget->move(0, -(value * 30)); });
}

bool OptionSettings::eventFilter(QObject *obj, QEvent *event) {
  if (obj == t_widget && event->type() == QEvent::Wheel) {
    auto wheelEvent = static_cast<QWheelEvent *>(event);
    int numDegrees = wheelEvent->angleDelta().y() / 8;
    int numSteps = numDegrees / 15;
    t_scrollbar->setValue(t_scrollbar->value() - numSteps);
  }
  if (obj == g_widget && event->type() == QEvent::Wheel) {
    auto wheelEvent = static_cast<QWheelEvent *>(event);
    int numDegrees = wheelEvent->angleDelta().y() / 8;
    int numSteps = numDegrees / 15;
    g_scrollbar->setValue(g_scrollbar->value() - numSteps);
  }
}