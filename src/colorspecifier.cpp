#include "colorspecifier.h"

ColorSpecifier::~ColorSpecifier() {
  // 释放资源
  // ...
}

ColorSpecifier::ColorSpecifier(const std::vector<QString> &color_names,
                               const std::vector<QColor *> &colors,
                               QWidget *parent)
    : QDialog(parent) {
  setFixedSize(window_width, window_height);
  QTableWidget *tableWidget = new QTableWidget(this);
  int color_nums = colors.size();
  // 设置行数和列数
  tableWidget->setRowCount(color_nums);
  tableWidget->setColumnCount(2);
  tableWidget->setColumnWidth(0, 150);
  tableWidget->setColumnWidth(1, 100);

  QStringList header;
  header << "项目"
         << "颜色";
  tableWidget->setHorizontalHeaderLabels(header);
  // 插入数据
  for (int i = 0; i < color_nums; ++i) {
    tableWidget->setItem(i, 0, new QTableWidgetItem(color_names[i]));
    ColorButton *btn = new ColorButton(colors[i]);
    tableWidget->setCellWidget(i, 1, btn);
  }
  tableWidget->verticalHeader()->hide();

  auto vlayout_ = new QVBoxLayout;
  vlayout_->addWidget(tableWidget, 1);
  setModal(true);
  setLayout(vlayout_);
}