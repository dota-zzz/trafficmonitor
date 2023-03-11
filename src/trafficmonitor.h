#pragma once
#include "basic.h"
#include "connetiondetails.h"
#include "hoverwidget.h"
#include "networkport.h"
#include "optionsettings.h"
#include <QBitmap>
#include <QColor>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPalette>
#include <QProcess>
#include <QPushButton>
#include <cstdlib>
#include <mutex>

class trafficmonitor : public QWidget {
  Q_OBJECT

public slots:
  void change_mainwindow_info(OptionInfo *info);

public:
  trafficmonitor(QWidget *parent = nullptr);
  ~trafficmonitor();
  void HoverWidgetShow();
  void MessageChanged();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void enterEvent(QEvent *event);
  void leaveEvent(QEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
  void create_ui();
  void create_logic();
  void open_optionsettings();
  void show_speed(double speed, QLabel *speed_label, QLabel *unit_label);
  void SentSpeedChanged();
  void ReceivedSpeedChanged();
  void CPUUsedChanged();
  void MemoryUesdChanged();
  void read_json();
  void write_json();
  void set_port_num(int num);
  void set_upellucidity(double upellucidity_);
  void set_fillet();
  void show_more_info();
  void hide_more_info();

  void init_label(QLabel *&label, const QString &str, QColor color);

  void make_more_info();
  void exchange_up_down();
  void change_over_always_top();
  int port_num = 0;
  QPoint m_dragPosition;
  QPoint m_Position;
  NetworkPort networkport;
  HoverWidget *hoverwidget;
  OptionSettings *options = nullptr;
  ConnectDetails *connect_details = nullptr;
  QLabel *download;
  QLabel *upload;
  QLabel *sent_speed;
  QLabel *received_speed;
  QLabel *sent_unit;
  QLabel *received_unit;
  QLabel *CPU;
  QLabel *memory;
  QLabel *CPU_mes;
  QLabel *memory_mes;
  QLabel *CPU_unit;
  QLabel *memory_unit;
  QPushButton *test;
  QMenu *menu;
  QColor mainwindow_color;
  QColor up_color;
  QColor down_color;
  QColor CPU_color;
  QColor memory_color;
  QFont mainwindow_font;
  QString up_text;
  QString down_text;
  QString CPU_text;
  QString memory_text;
  std::mutex mutex_;
  QHBoxLayout *up_down_layout;
  QHBoxLayout *cpu_memory_layout = nullptr;
  QVBoxLayout *mainwindow_layout;
  QWidget *hlist1;
  QWidget *hlist2;
  QWidget *hlist3;
  QWidget *hlist4;
  QAction *lock_window_pos;
  QAction *always_top;
  QAction *lshow_more_info;
  CPUMemoryTask *cpu_memory_task = nullptr;
  // settings
  bool is_always_top;
  bool is_showing_more_info;
  bool is_lock_window_pos;
  bool is_mainwindow_show;
  double upellucidity;
  bool is_specify_color;
  bool is_exchange_up_down;
  bool is_concise_mode;
  bool is_show_mouse_tips;
  int memory_show_mode;
  int network_unit;
  int unit_choose;
  int double_click_action;
  bool is_show_network_unit;
  bool is_show_percent_sign;
  bool allow_to_exceed;
};