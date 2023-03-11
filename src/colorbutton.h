#pragma once

#include "basic.h"
#include <QColor>
#include <QColorDialog>
#include <QMouseEvent>
#include <QPushButton>

class ColorButton : public QPushButton {
  Q_OBJECT

public:
  ColorButton(QColor *main, QWidget *parent = nullptr);
  ~ColorButton();

protected:
  void mousePressEvent(QMouseEvent *event) override;

private:
  QColor *main_color;
};