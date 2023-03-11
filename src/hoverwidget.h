#pragma once

#include "mmemory.h"
#include "networkport.h"
#include <QFont>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <functional>
#include <thread>

class HoverWidget : public QWidget {
  Q_OBJECT

protected:
  void paintEvent(QPaintEvent *event) override;

public:
  HoverWidget(NetworkPort *, int, QWidget *parent = nullptr);
  ~HoverWidget() { delete timer; }
  void setOnHoverWidgetShow(std::function<void()> callback);
  void reset_timer();
  void close_timer();
  void set_traffic();
  void set_source();
  std::function<void()> OnHoverWidgetShow;

private:
  void create_ui();
  void create_logic();
  void switch_unit(QLabel *unit, double &traffic);

  NetworkPort *networkport;
  int port_num = 0;
  QTimer *timer;
  QLabel *traffic_used_today_label;
  QLabel *traffic_used_sent_label;
  QLabel *traffic_used_received_label;
  QLabel *CPU_used_label;
  QLabel *memory_used_label;
  QLabel *memory_percentage_label;
  QLabel *traffic_used_unit;
  QLabel *traffic_sent_unit;
  QLabel *traffic_received_unit;
};