#include "sendwidget.h"
#include <QColorDialog>
#include <QDataStream>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

SendWidget::SendWidget(QWidget *parent) : QWidget(parent) {
  pColour = Qt::black;
  pWidth = 1;
}

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
  painter.setPen(
      QPen(pColour, pWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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

void SendWidget::fillScreen() {
  mPoints.clear();
  image.fill(pColour);
  updateImage();
  sendImage();
}

void SendWidget::penWidth(int width) { pWidth = width; }

void SendWidget::penColour(int colour) {
  switch (colour) {
  case 0:
    pColour = Qt::black;
    break;
  case 1:
    pColour = Qt::red;
    break;
  case 2:
    pColour = Qt::green;
    break;
  case 3:
    pColour = Qt::blue;
    break;
  case 4:
    pColour = Qt::cyan;
    break;
  case 5:
    pColour = Qt::magenta;
    break;
  case 6:
    pColour = Qt::yellow;
    break;
  case 7:
    pColour = QColorDialog::getColor(pColour, this);
    break;
  default:
    pColour = Qt::black;
    break;
  }
}


void SendWidget::openImg(){
  QString filename = QFileDialog::getOpenFileName(this, "Open Image");
  if(filename != 0){
  mPoints.clear();
  image.load(filename);
  QSize newSize = image.size();
  if(newSize.width() < 530){
    newSize.setWidth(530);
  }
  if(newSize.height() < 350){
    newSize.setHeight(350);
  }
  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(Qt::white);
  QPainter painter(&newImage);
  painter.drawImage(0, 0, image);
  image = newImage;
  this->update();
  sendImage();
  }
}

void SendWidget::saveImg(){
  QString filename = QFileDialog::getSaveFileName(this, "Save Image");
  if(filename != 0){
  image.save(filename, 0, -1);
  }
}
