#pragma once

#include "networkport.h"
#include <QCloseEvent>
#include <QDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QPushButton>
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class ConnectDetails : public QDialog {
  Q_OBJECT

protected:
  void closeEvent(QCloseEvent *event) override;

public:
  ConnectDetails(ConnectDetails *&ptr, NetworkPort::PortDetails *port_details,
                 QWidget *parent = nullptr);
  ~ConnectDetails();

private:
  ConnectDetails *&connect_details_ptr;
  const int window_width = 550;
  const int window_height = 350;
};