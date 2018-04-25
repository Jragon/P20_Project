#ifndef SENDWIDGET_H
#define SENDWIDGET_H

#include <QWidget>
#include "package.h"

class SendWidget : public QWidget {
  Q_OBJECT
 public:
  explicit SendWidget(QWidget *parent = nullptr);

 signals:
  void send(package_t *pkg);

 public slots:

 private:
  QVector<QPointF> mPoints;
  QImage image;

  void updateImage();
  void sendImage();

 protected:
  void paintEvent(QPaintEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void resizeEvent(QResizeEvent *e);
};

#endif  // SENDWIDGET_H
