#ifndef RECEIVEWIDGET_H
#define RECEIVEWIDGET_H

#include <QWidget>
#include "package.h"

class ReceiveWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ReceiveWidget(QWidget *parent = nullptr);

 signals:

 public slots:
  void receive(package_t pkg);

 private:
  QImage image;

 protected:
  void paintEvent(QPaintEvent *e);
};

#endif  // RECEIVEWIDGET_H
