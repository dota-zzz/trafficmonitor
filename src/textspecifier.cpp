#include "textspecifier.h"

TextSpecifier::~TextSpecifier() {}

void TextSpecifier::closeEvent(QCloseEvent *event) {
  for (int i = 0; i < vle.size(); ++i) {
    *(texts[i]) = (vle[i])->text();
  }
}
TextSpecifier::TextSpecifier(const std::vector<QString> &text_names,
                             const std::vector<QString *> &texts_,
                             QWidget *parent)
    : QDialog(parent), texts(texts_) {
  setFixedSize(window_width, window_height);
  QTableWidget *tableWidget = new QTableWidget(this);
  int text_nums = texts_.size();
  // 设置行数和列数
  tableWidget->setRowCount(text_nums);
  tableWidget->setColumnCount(2);
  tableWidget->setColumnWidth(0, 150);
  tableWidget->setColumnWidth(1, 100);

  QStringList header;
  header << "项目"
         << "值";
  tableWidget->setHorizontalHeaderLabels(header);
  // 插入数据
  for (int i = 0; i < text_nums; ++i) {
    tableWidget->setItem(i, 0, new QTableWidgetItem(text_names[i]));
    vle.push_back(new QLineEdit);
    vle[i]->setText(*(texts_[i]));
    tableWidget->setCellWidget(i, 1, vle[i]);
    // connect(le, &QLineEdit::textChanged, [&](const QString &text) {
    //     *(texts[i]) = text;
    // });
  }
  tableWidget->verticalHeader()->hide();
  setAttribute(Qt::WA_DeleteOnClose);
  auto vlayout_ = new QVBoxLayout;
  vlayout_->addWidget(tableWidget, 1);
  setModal(true);
  setLayout(vlayout_);
}