#include "specifybutton.h"

SpecifyButton::~SpecifyButton() {}

void SpecifyButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);
  QPainter painter(this);
  if (mode == 0) {
    QPainter painter(this);
    painter.fillRect(rect(), *main_color);
  } else if (mode == 1) { // 绘制左上部分为红色
    QRect topLeftRect = QRect(0, 0, width() / 2, height() / 2);
    painter.fillRect(topLeftRect, *(colors[0]));

    // 绘制右上部分为蓝色
    QRect topRightRect = QRect(width() / 2, 0, width() / 2, height() / 2);
    painter.fillRect(topRightRect, *(colors[1]));

    // 绘制左下部分为黄色
    QRect bottomLeftRect = QRect(0, height() / 2, width() / 2, height() / 2);
    painter.fillRect(bottomLeftRect, *(colors[2]));

    // 绘制右下部分为绿色
    QRect bottomRightRect =
        QRect(width() / 2, height() / 2, width() / 2, height() / 2);
    painter.fillRect(bottomRightRect, *(colors[3]));
  }
}
void SpecifyButton::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    if (mode == 0) {
      QColorDialog color;
      QColor c = color.getColor();
      set_button_color(this, c);
      *main_color = c;
    } else if (mode == 1) {
      auto color_details = new ColorSpecifier(color_names, colors);
      color_details->setAttribute(Qt::WA_DeleteOnClose);
      color_details->show();
    }
  }
}