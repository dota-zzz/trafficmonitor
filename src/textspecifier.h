#pragma once

#include "colorbutton.h"
#include <QCloseEvent>
#include <QDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <string>
#include <vector>


class TextSpecifier : public QDialog {
  Q_OBJECT

protected:
  void closeEvent(QCloseEvent *event) override;

public:
  TextSpecifier(const std::vector<QString> &text_names,
                const std::vector<QString *> &texts_,
                QWidget *parent = nullptr);

  ~TextSpecifier();

private:
  const int window_width = 300;
  const int window_height = 200;
  std::vector<QLineEdit *> vle;
  std::vector<QString *> texts;
};