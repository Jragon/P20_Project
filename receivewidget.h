#ifndef RECEIVEWIDGET_H
#define RECEIVEWIDGET_H

#include <QWidget>
#include "package.h"

class ReceiveWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ReceiveWidget(QWidget *parent = 0);

 signals:

 public slots:
  void receive(package_t pkg);
  void saveImg();

 private:
  QImage image;

 protected:
  void paintEvent(QPaintEvent *e);
};

#endif  // RECEIVEWIDGET_H
