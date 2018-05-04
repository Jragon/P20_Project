#include "receivewidget.h"
#include <QDataStream>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QFileDialog>

ReceiveWidget::ReceiveWidget(QWidget *parent) : QWidget(parent) {}
void ReceiveWidget::receive(package_t pkg) {
  switch (pkg.cmd) {
    case package_t::Image: {
      QDataStream stream(pkg.data);
      stream >> image;
      this->update();
      break;
    }
    default:
      break;
  }
}

void ReceiveWidget::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  QStyleOption options;
  options.initFrom(this);
  this->style()->drawPrimitive(QStyle::PE_Widget, &options, &painter, this);

  if (image.width() != this->width() || image.height() != this->height())
    if (image.width() > 0 && image.height() > 0) this->resize(image.size());

  painter.drawImage(e->rect().topLeft(), image);
}

void ReceiveWidget::saveImg(){
  QString filename = QFileDialog::getSaveFileName(this, "Save Image");
  if(filename != 0){
  image.save(filename, 0, -1);
  }
}
