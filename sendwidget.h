#ifndef SENDWIDGET_H
#define SENDWIDGET_H

#include <QWidget>
#include "package.h"

class SendWidget : public QWidget {
  Q_OBJECT
 public:
  explicit SendWidget(QWidget *parent = 0);

 signals:
  void send(package_t pkg);

 public slots:
  void clearScreen();
  void penColour(int colour);
  void penWidth(int width);

 private:
  QVector<QPointF> mPoints;
  QImage image;
  QColor pColour;
  int pWidth;

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
