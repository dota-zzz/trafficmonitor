#pragma once

#include "HlayoutPair.h"
#include "basic.h"
#include "colorbutton.h"
#include "specifybutton.h"
#include "textspecifier.h"
#include <QButtonGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QColor>
#include <QColorDialog>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollBar>
#include <QSpinBox>
#include <QTabWidget>

class OptionSettings : public QDialog {
  Q_OBJECT

signals:
  void send_mainwindow_info(OptionInfo *);

public:
  // OptionSettings(QWidget *parent = Q_NULLPTR);
  OptionSettings(OptionSettings *&ptr, OptionInfo *_option_info,
                 QWidget *parent = Q_NULLPTR);
  ~OptionSettings() {}

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
  void closeEvent(QCloseEvent *event) override {
    optionsettings_ptr = nullptr;
    delete option_info;
  }

private:
  void create_ui();
  void create_logic();
  // 基础设置
  OptionInfo *option_info;
  OptionSettings *&optionsettings_ptr;
  const int window_width = 500;
  const int window_height = 700;
  QTabWidget *tab_widget;
  QPushButton *btn1;
  QPushButton *btn2;
  QPushButton *btn3;
  // 主窗口设置
  QWidget *create_mwindowset();
  QWidget *create_mwindowset_fontcolor();
  QWidget *create_mwindowset_showtext();
  QWidget *create_mwindowset_other();
  // 任务栏窗口设置
  QWidget *create_tbarwindowset();
  QWidget *create_tbarwindowset_fontcolor();
  QWidget *create_tbarwindowset_displayset();
  QWidget *create_tbarwindowset_tbarwindowset();
  QWidget *create_tbarwindowset_sourceuchart();
  // 常规设置
  QWidget *create_generalset();
  QWidget *create_generalset_appset();
  QWidget *create_generalset_configdata();
  QWidget *create_generalset_inform();
  QWidget *create_generalset_hwaremonitor();
  QWidget *create_generalset_seniorset();
  QWidget *create_generalset_display();
  // 共用函数
  QWidget *create_unitset(QRadioButton *&, QRadioButton *&, QButtonGroup *&,
                          HlayoutPair<QLabel, MyComboBox> *&, QCheckBox *&,
                          QCheckBox *&);
  void get_scrollbar(QScrollBar *&, QWidget *, QWidget *);
  // 主窗口设置
  // 颜色和字体
  HlayoutPair<QLabel, QPushButton> *m_f_hpair1;
  HlayoutPair<QLabel, MySpinBox> *m_f_hpair2;
  QWidget *m_f_hpair3;
  SpecifyButton *m_f_cb;
  QCheckBox *m_f_cbox1;
  // 显示文本
  QPushButton *m_s_btn1;
  QCheckBox *m_s_cbox1;
  QCheckBox *m_s_cbox2;
  // QCheckBox *m_s_cbox3;
  QCheckBox *m_s_cbox4;
  HlayoutPair<QLabel, MyComboBox> *m_s_pair1;
  // 单位设置
  QRadioButton *m_u_rbtn1;
  QRadioButton *m_u_rbtn2;
  QButtonGroup *m_u_bgroup;
  HlayoutPair<QLabel, MyComboBox> *m_u_hpair1;
  QCheckBox *m_u_cbox1;
  QCheckBox *m_u_cbox2;
  // 其他
  QCheckBox *m_o_cbox1;
  QCheckBox *m_o_cbox2;
  QCheckBox *m_o_cbox3;
  QCheckBox *m_o_cbox4;
  // QCheckBox *m_o_cbox5;
  HlayoutPair<QLabel, MyComboBox> *m_o_hpair1;

  // 任务栏窗口设置
  QScrollBar *t_scrollbar;
  QWidget *t_widget;
  QWidget *t_m_widget;
  // 颜色和字体
  HlayoutPair<QLabel, QPushButton> *t_f_hpair1;
  HlayoutPair<QLabel, MySpinBox> *t_f_hpair2;
  HlayoutPair<QLabel, QPushButton> *t_f_hpair3;
  QCheckBox *t_f_cbox1;
  HlayoutPair<QLabel, QPushButton> *t_f_hpair4;
  QCheckBox *t_f_cbox2;
  QCheckBox *t_f_cbox3;
  // 显示设置
  QPushButton *t_d_btn1;
  QPushButton *t_d_btn2;
  QCheckBox *t_d_cbox1;
  QCheckBox *t_d_cbox2;
  QCheckBox *t_d_cbox3;
  QCheckBox *t_d_cbox4;
  QCheckBox *t_d_cbox5;
  MyComboBox *t_d_cbobox1;
  MyComboBox *t_d_cbobox2;
  MySpinBox *t_d_sbox1;
  // 单位设置
  QRadioButton *t_u_rbtn1;
  QRadioButton *t_u_rbtn2;
  QButtonGroup *t_u_bgroup;
  HlayoutPair<QLabel, MyComboBox> *t_u_hpair1;
  QCheckBox *t_u_cbox1;
  QCheckBox *t_u_cbox2;
  // 任务栏窗口
  QCheckBox *t_t_cbox1;
  QCheckBox *t_t_cbox2;
  MyComboBox *t_t_cbobox1;
  // 资源占用图
  QCheckBox *t_r_cbox1;
  QCheckBox *t_r_cbox2;
  QCheckBox *t_r_cbox3;
  MySpinBox *t_r_sbox1;
  MyComboBox *t_r_cbobox1;
  QPushButton *t_r_btn1;
  QRadioButton *t_r_rbtn1;
  QRadioButton *t_r_rbtn2;

  // 常规设置
  QScrollBar *g_scrollbar;
  QWidget *g_widget;
  QWidget *g_m_widget;
  // 应用程序设置
  QCheckBox *g_a_cbox1;
  QPushButton *g_a_btn1;
  QRadioButton *g_a_rbtn1;
  QRadioButton *g_a_rbtn2;
  QCheckBox *g_a_cbox2;
  QPushButton *g_a_btn2;
  MyComboBox *g_a_cbobox1;
  // 配置和数据文件
  QRadioButton *g_c_rbtn1;
  QRadioButton *g_c_rbtn2;
  QPushButton *g_c_btn1;
  // 通知消息
  QCheckBox *g_i_cbox1;
  QCheckBox *g_i_cbox2;
  QCheckBox *g_i_cbox3;
  QCheckBox *g_i_cbox4;
  QCheckBox *g_i_cbox5;
  QCheckBox *g_i_cbox6;
  MySpinBox *g_i_sbox1;
  MySpinBox *g_i_sbox2;
  MySpinBox *g_i_sbox3;
  MySpinBox *g_i_sbox4;
  MySpinBox *g_i_sbox5;
  MySpinBox *g_i_sbox6;
  MyComboBox *g_i_cbobox1;
  // 硬件监控
  QCheckBox *g_h_cbox1;
  QCheckBox *g_h_cbox2;
  QCheckBox *g_h_cbox3;
  QCheckBox *g_h_cbox4;
  MyComboBox *g_h_sbox1;
  MyComboBox *g_h_sbox2;
  // 高级
  QCheckBox *g_ad_cbox1;
  QPushButton *g_ad_btn1;
  QRadioButton *g_ad_rbtn1;
  QRadioButton *g_ad_rbtn2;
  MySpinBox *g_ad_sbox1;
  QPushButton *g_ad_btn2;
  QPushButton *g_ad_btn3;
  // 显示
  QCheckBox *g_d_cbox1;
  QCheckBox *g_d_cbox2;
  QCheckBox *g_d_cbox3;
};