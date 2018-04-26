#include "sendwidget.h"
#include <QDataStream>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

SendWidget::SendWidget(QWidget *parent) : QWidget(parent) {}

void SendWidget::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  QStyleOption options;
  options.initFrom(this);
  this->style()->drawPrimitive(QStyle::PE_Widget, &options, &painter, this);

  painter.drawImage(e->rect().topLeft(), image);
}

void SendWidget::mousePressEvent(QMouseEvent *e) {
  mPoints.clear();
  mPoints.append(e->localPos());
  updateImage();
}

void SendWidget::mouseMoveEvent(QMouseEvent *e) {
  mPoints.append(e->localPos());
  updateImage();
}

void SendWidget::mouseReleaseEvent(QMouseEvent *e) {
  mPoints.append(e->localPos());
  updateImage();
  sendImage();
}

void SendWidget::updateImage() {
  QPainter painter(&image);
  painter.setPen(QColor(Qt::black));
  painter.setRenderHint(QPainter::Antialiasing);
  painter.drawPolyline(mPoints.data(), mPoints.count());

  this->update();
}

void SendWidget::sendImage() {
  package_t pkg;
  pkg.cmd = package_t::Image;
  QDataStream stream(&pkg.data, QIODevice::WriteOnly);
  stream << image;

  emit send(pkg);
}

// void SendWidget::addPoint(QPointF point) {
//  package_t pkg;
//  pkg.cmd = package_t::Point;
//  pkg.data.appen emit send(pkg);
//}

void SendWidget::resizeEvent(QResizeEvent *e) {
  QImage newImage(e->size(), QImage::Format_RGB32);
  newImage.fill(Qt::white);

  QPainter painter(&newImage);
  painter.drawImage(0, 0, image);
  image = newImage;

  QWidget::resizeEvent(e);
}

void SendWidget::clearScreen() {
  mPoints.clear();
  image.fill(Qt::white);
  updateImage();
  sendImage();
}
