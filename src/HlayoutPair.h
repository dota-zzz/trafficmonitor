#ifndef MYTRAFFICMONITOR_HLAYOUTPAIR_
#define MYTRAFFICMONITOR_HLAYOUTPAIR_

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

template <typename First, typename Second> class HlayoutPair : public QWidget {
public:
  HlayoutPair();
  ~HlayoutPair();
  First *first;
  Second *second;
};

template <typename First, typename Second>
HlayoutPair<First, Second>::HlayoutPair() {
  first = new First;
  second = new Second;

  auto h_layout = new QHBoxLayout(this);

  h_layout->addWidget(first, 2);
  h_layout->addWidget(second, 3);
  h_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(h_layout);
}

template <typename First, typename Second>
HlayoutPair<First, Second>::~HlayoutPair() {
  delete first;
  delete second;
}

template <typename... Args> QWidget *make_hwidgetlist(Args... args) {
  auto hlayout_ = new QHBoxLayout;
  (hlayout_->addWidget(args), ...);
  hlayout_->setContentsMargins(0, 0, 0, 0);
  auto widget_ = new QWidget;
  widget_->setLayout(hlayout_);
  return widget_;
}

template <typename... Args>
QWidget *make_hwidgetlist_flabel(const QString &s, Args... args) {
  auto hlayout_ = new QHBoxLayout;
  auto label_ = new QLabel(s);
  hlayout_->addWidget(label_);
  (hlayout_->addWidget(args), ...);
  hlayout_->setContentsMargins(0, 0, 0, 0);
  auto widget_ = new QWidget;
  widget_->setLayout(hlayout_);
  return widget_;
}

template <typename Second> class HlayoutPair<QLabel, Second> : public QWidget {
public:
  HlayoutPair();
  HlayoutPair(const QString &);
  ~HlayoutPair();
  QLabel *first;
  Second *second;
};

template <typename Second>
HlayoutPair<QLabel, Second>::HlayoutPair(const QString &s) {
  first = new QLabel(s);
  second = new Second;

  auto h_layout = new QHBoxLayout(this);

  h_layout->addWidget(first, 2);
  h_layout->addWidget(second, 3);
  h_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(h_layout);
}

template <typename Second> HlayoutPair<QLabel, Second>::HlayoutPair() {
  first = new QLabel;
  second = new Second;

  auto h_layout = new QHBoxLayout(this);

  h_layout->addWidget(first, 2);
  h_layout->addWidget(second, 3);
  h_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(h_layout);
}

template <typename Second> HlayoutPair<QLabel, Second>::~HlayoutPair() {
  delete first;
  delete second;
}

#endif