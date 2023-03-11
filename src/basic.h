#pragma once

#include <QColor>
#include <QFrame>
#include <QJsonArray>
#include <QLabel>
#include <QLayout>
#include <QPalette>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>

class MyComboBox : public QComboBox {
public:
  MyComboBox(QWidget *parent = nullptr) : QComboBox(parent) {}

protected:
  void wheelEvent(QWheelEvent *event) override {
    // 调用父类的滚轮事件处理函数，保留原有的滚轮事件处理功能
    // QComboBox::wheelEvent(event);
    // 在此添加自己的滚轮事件处理代码
    // ...
    event->ignore();
  }
};

class MySpinBox : public QSpinBox {
public:
  MySpinBox(QWidget *parent = nullptr) : QSpinBox(parent) {}

protected:
  void wheelEvent(QWheelEvent *event) override {
    // 调用父类的滚轮事件处理函数，保留原有的滚轮事件处理功能
    // QComboBox::wheelEvent(event);
    // 在此添加自己的滚轮事件处理代码
    // ...
    event->ignore();
  }
};

struct OptionInfo {
  QColor omainwindow_color;
  QFont omainwindow_font;
  QColor up_color;
  QColor down_color;
  QColor CPU_color;
  QColor memory_color;
  QString up_text;
  QString down_text;
  QString CPU_text;
  QString memory_text;
  bool ois_specify_color;
  bool ois_exchange_up_down;
  bool ois_concise_mode;
  bool ois_show_network_unit;
  bool ois_show_percent_sign;
  bool ois_show_mouse_tips;
  bool ois_always_top;
  bool ois_lock_window_pos;
  bool ois_showing_more_info;
  bool oallow_to_exceed;
  int onetwork_unit;
  int ounit_choose;
  int omemory_show_mode;
  int odouble_click_action;
  OptionInfo() {}
  OptionInfo(const OptionInfo &info) {
    omainwindow_color = info.omainwindow_color;
    omainwindow_font = info.omainwindow_font;
    up_color = info.up_color;
    down_color = info.down_color;
    CPU_color = info.CPU_color;
    memory_color = info.memory_color;
    up_text = info.up_text;
    down_text = info.down_text;
    CPU_text = info.CPU_text;
    memory_text = info.memory_text;
    ois_specify_color = info.ois_specify_color;
    ois_exchange_up_down = info.ois_exchange_up_down;
    ois_concise_mode = info.ois_concise_mode;
    ois_show_network_unit = info.ois_show_network_unit;
    ois_show_percent_sign = info.ois_show_percent_sign;
    ois_show_mouse_tips = info.ois_show_mouse_tips;
    ois_always_top = info.ois_always_top;
    ois_lock_window_pos = info.ois_lock_window_pos;
    ois_showing_more_info = info.ois_showing_more_info;
    oallow_to_exceed = info.oallow_to_exceed;
    onetwork_unit = info.onetwork_unit;
    ounit_choose = info.ounit_choose;
    omemory_show_mode = info.omemory_show_mode;
    odouble_click_action = info.odouble_click_action;
  }
};

template <typename T> static void set_label_color(T *t, QColor color) {
  QPalette palette = t->palette();
  palette.setColor(QPalette::WindowText, color);
  t->setPalette(palette);
}

static void set_button_color(QPushButton *button, QColor color) {
  int r = color.red();   // 获取颜色的红色分量
  int g = color.green(); // 获取颜色的绿色分量
  int b = color.blue();  // 获取颜色的蓝色分量

  QString style =
      QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b);
  button->setStyleSheet(style);
}

static void clear_layout(QLayout *layout) {
  while (QLayoutItem *item = layout->takeAt(0)) {
    if (QWidget *widget = item->widget())
      widget->deleteLater();

    if (QLayout *childLayout = item->layout())
      clear_layout(childLayout);

    if (QSpacerItem *spaerItem = item->spacerItem())
      layout->removeItem(spaerItem);

    delete item;
  }
}

template <typename T> static void set_fixed_size(T *widget, int x, int y) {
  widget->setMinimumSize(QSize(x, y));
  widget->setMaximumSize(QSize(x, y));
}

static void make_color(QColor &color, const QJsonArray &array) {
  if (array.size() == 4) {
    int r = array[0].toInt();
    int g = array[1].toInt();
    int b = array[2].toInt();
    int a = array[3].toInt();
    color = QColor(r, g, b, a);
    // 使用创建的QColor对象
  }
}

static QJsonArray save_color(const QColor &color) {
  int r, g, b, a;
  color.getRgb(&r, &g, &b, &a);
  QJsonArray rgbaArray;
  rgbaArray.append(r);
  rgbaArray.append(g);
  rgbaArray.append(b);
  rgbaArray.append(a);
  return rgbaArray;
}