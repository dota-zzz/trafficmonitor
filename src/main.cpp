#include "hoverwidget.h"
#include "networkport.h"
#include "optionsettings.h"
#include "trafficmonitor.h"
#include "connetiondetails.h"
#include <QApplication>
#include <iostream>
#include <windows.h>
#include <string>
#include "mmemory.h"
#include <Windows.h>
#include <QFont>
#include <QFontDialog>
#include <QPalette>
#include <QWidget>
#include <QFont>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <thread>
#include <QColor>
#include <QGridLayout>
#include <QProcess>
#include <cstdlib>

int main(int argc, char *argv[])
{

  QApplication a(argc, argv);
  trafficmonitor w;
  w.show();
  return a.exec();

}
