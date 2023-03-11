#pragma once

#include "basic.h"
#include "colorspecifier.h"
#include <QColor>
#include <QColorDialog>
#include <QFrame>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <string>
#include <vector>

class SpecifyButton : public QPushButton {
  Q_OBJECT
protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

public:
  SpecifyButton(QColor *main, int mode_, std::vector<QString> color_names_,
                std::vector<QColor *> colors_, QWidget *parent = nullptr)
      : main_color(main), color_names(color_names_), colors(colors_) {
    mode = mode_;
  }
  ~SpecifyButton();

  void set_specify_mode() {
    mode = 1;
    update();
  }

  void set_common_mode() {
    mode = 0;
    update();
  }

private:
  int mode = 0;
  QColor *main_color;
  std::vector<QString> color_names;
  std::vector<QColor *> colors;
  QGridLayout *specify_layout = nullptr;
};