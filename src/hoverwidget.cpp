#include "hoverwidget.h"
#include "HlayoutPair.h"
#include <QVBoxLayout>
#include <Windows.h>
#include <cmath>

HoverWidget::HoverWidget(NetworkPort *ntport, int ptnum, QWidget *parent) {
  networkport = ntport;
  port_num = ptnum;
  create_ui();
  QFont f("楷体");
  f.setPointSize(8);
  setFont(f);
  create_logic();
}

void HoverWidget::create_ui() {
  this->setWindowFlags(Qt::FramelessWindowHint);
  this->setFixedSize(250, 100);

  traffic_used_unit = new QLabel("KB");
  traffic_sent_unit = new QLabel("KB");
  traffic_received_unit = new QLabel("KB");

  traffic_used_today_label = new QLabel;
  traffic_used_sent_label = new QLabel;
  traffic_used_received_label = new QLabel;
  set_traffic();

  double CPU_used = get_CPU_used();
  CPU_used_label = new QLabel(QString::number(CPU_used, 'f', 2) + "%");

  auto &&memory_vec = get_memory_message();
  double memory_used = memory_vec[2];
  memory_used_label = new QLabel(QString::number(memory_used, 'f', 2) + "GB");

  auto hlist1_ = make_hwidgetlist_flabel(
      "今日已使用流量:", traffic_used_today_label, traffic_used_unit);

  auto label2_1_ = new QLabel("/下载:");
  auto label2_2_ = new QLabel(")");
  auto hlist2_ = make_hwidgetlist_flabel(
      "(上传:", traffic_used_sent_label, traffic_sent_unit, label2_1_,
      traffic_used_received_label, traffic_received_unit, label2_2_);

  auto hlist3_ = make_hwidgetlist_flabel("CPU利用率:", CPU_used_label);

  double memory = memory_vec[0];
  double percent_memory = memory_used / memory;
  auto total_memory_ = new QLabel("/" + QString::number(memory, 'f', 2) + "GB");
  memory_percentage_label =
      new QLabel(QString::number(percent_memory * 100, 'f', 2) + "%");
  auto hlist4_ = make_hwidgetlist_flabel(
      "内存利用率:", memory_used_label, total_memory_, memory_percentage_label);

  auto vlayout_ = new QVBoxLayout;
  vlayout_->addWidget(hlist1_);
  vlayout_->addWidget(hlist2_);
  vlayout_->addWidget(hlist3_);
  vlayout_->addWidget(hlist4_);

  this->setLayout(vlayout_);
}

void HoverWidget::create_logic() {
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, [&]() { OnHoverWidgetShow(); });
}

void HoverWidget::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.setPen(QColor("green"));                   // 设置画笔记颜色
  p.drawRect(0, 0, width() - 1, height() - 1); // 绘制边框
}
void HoverWidget::reset_timer() {
  if (timer->isActive()) {
    timer->stop();
  }
  timer->start(500);
}
void HoverWidget::close_timer() {
  if (timer->isActive()) {
    timer->stop();
  }
}
void HoverWidget::set_traffic() {
  double traffic_used_sent = networkport->get_sent(port_num);
  double traffic_used_received = networkport->get_received(port_num);
  double traffic_used = traffic_used_sent + traffic_used_received;
  switch_unit(traffic_sent_unit, traffic_used_sent);
  switch_unit(traffic_received_unit, traffic_used_received);
  switch_unit(traffic_used_unit, traffic_used);
  traffic_used_today_label->setText(QString::number(traffic_used, 'f', 2));
  traffic_used_sent_label->setText(QString::number(traffic_used_sent, 'f', 2));
  traffic_used_received_label->setText(
      QString::number(traffic_used_received, 'f', 2));
}
void HoverWidget::set_source() {
  auto &&memory_vec = get_memory_message();
  double memory = memory_vec[0];
  double memory_used = memory_vec[2];
  double percent_memory = memory_used / memory;
  memory_percentage_label->setText(
      QString::number(percent_memory * 100, 'f', 2) + "%");
  std::thread set_CPU_tast([&]() {
    double CPU_used = get_CPU_used();
    CPU_used_label->setText(QString::number(CPU_used, 'f', 2) + "%");
  });
  set_CPU_tast.detach();
}
void HoverWidget::switch_unit(QLabel *unit, double &traffic) {
  if (traffic < 1024) {
    unit->setText("KB");
  } else if (traffic < 1024 * 1024) {
    unit->setText("MB");
    traffic /= 1024;
  } else {
    unit->setText("GB");
    traffic /= (1024 * 1024);
  }
}

void HoverWidget::setOnHoverWidgetShow(std::function<void()> callback) {
  OnHoverWidgetShow = callback;
}