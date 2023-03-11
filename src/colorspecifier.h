#pragma once

#include "colorbutton.h"
#include <QCloseEvent>
#include <QDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <string>
#include <vector>

class ColorSpecifier : public QDialog {
  Q_OBJECT

protected:
public:
  ColorSpecifier(const std::vector<QString> &color_names,
                 const std::vector<QColor *> &colors,
                 QWidget *parent = nullptr);
  ~ColorSpecifier();

private:
  const int window_width = 300;
  const int window_height = 200;
};