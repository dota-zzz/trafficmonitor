#include "colorbutton.h"

ColorButton::~ColorButton() {}

ColorButton::ColorButton(QColor *main, QWidget *parent) : main_color(main) {
  set_button_color(this, *main_color);
}
void ColorButton::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    QColorDialog color;
    QColor c = color.getColor();
    set_button_color(this, c);
    *main_color = c;
  }
}