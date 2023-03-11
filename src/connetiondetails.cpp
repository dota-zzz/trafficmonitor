#include "connetiondetails.h"
#include <QHeaderView>
#include <QStringList>
#include <QTableWidget>

ConnectDetails::~ConnectDetails() {
  // 释放资源
  // ...
}

void ConnectDetails::closeEvent(QCloseEvent *event) {
  connect_details_ptr = nullptr;
}

ConnectDetails::ConnectDetails(ConnectDetails *&ptr,
                               NetworkPort::PortDetails *port_details,
                               QWidget *parent)
    : QDialog(parent), connect_details_ptr(ptr) {
  setFixedSize(window_width, window_height);
  QTableWidget *tableWidget = new QTableWidget(this);

  // 设置行数和列数
  tableWidget->setRowCount(8);
  tableWidget->setColumnCount(2);
  tableWidget->setColumnWidth(0, 150);
  tableWidget->setColumnWidth(1, 350);

  QStringList header;
  header << "项目"
         << "值";
  tableWidget->setHorizontalHeaderLabels(header);
  // 插入数据
  tableWidget->setItem(0, 0, new QTableWidgetItem("接口名"));
  tableWidget->setItem(1, 0, new QTableWidgetItem("接口描述"));
  tableWidget->setItem(2, 0, new QTableWidgetItem("连接类型"));
  tableWidget->setItem(3, 0, new QTableWidgetItem("速度"));
  tableWidget->setItem(4, 0, new QTableWidgetItem("适配器物理地址"));
  tableWidget->setItem(5, 0, new QTableWidgetItem("IP地址"));
  tableWidget->setItem(6, 0, new QTableWidgetItem("子网掩码"));
  tableWidget->setItem(7, 0, new QTableWidgetItem("默认网关"));
  tableWidget->setItem(
      0, 1,
      new QTableWidgetItem(QString::fromStdString(port_details->adapter_name)));
  tableWidget->setItem(1, 1,
                       new QTableWidgetItem(QString::fromStdString(
                           port_details->adapter_description)));
  tableWidget->setItem(
      2, 1,
      new QTableWidgetItem(QString::fromStdString(port_details->adapter_type)));
  tableWidget->setItem(
      3, 1,
      new QTableWidgetItem(
          QString::number(port_details->adapter_speed / (1000 * 1000)) +
          "Mbps"));
  tableWidget->setItem(
      4, 1,
      new QTableWidgetItem(QString::fromStdString(port_details->mac_addresse)));
  tableWidget->setItem(
      5, 1,
      new QTableWidgetItem(QString::fromStdString(port_details->ip_address)));
  tableWidget->setItem(
      6, 1,
      new QTableWidgetItem(QString::fromStdString(port_details->subnet_mask)));
  tableWidget->setItem(7, 1,
                       new QTableWidgetItem(QString::fromStdString(
                           port_details->default_gateway)));
  tableWidget->verticalHeader()->hide();

  auto btn1 = new QPushButton("关闭");
  connect(btn1, &QPushButton::clicked, this, &ConnectDetails::close);
  auto hlayout_ = new QHBoxLayout;
  hlayout_->addStretch(1);
  hlayout_->addWidget(btn1);
  auto widget1_ = new QWidget;
  widget1_->setLayout(hlayout_);

  auto vlayout_ = new QVBoxLayout;
  vlayout_->addWidget(tableWidget, 1);
  vlayout_->addWidget(widget1_);
  // auto centralwidget = new QWidget(this);
  // centralwidget->setLayout(vlayout_);

  // setCentralWidget(centralwidget);
  setModal(true);
  setLayout(vlayout_);

  delete port_details;
}