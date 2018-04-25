#include "receivewidget.h"
#include <QDataStream>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

ReceiveWidget::ReceiveWidget(QWidget *parent) : QWidget(parent) {}

void ReceiveWidget::receive(package_t *pkg) {
  switch (pkg->cmd) {
    case package_t::Image: {
      QDataStream stream(pkg->data);
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

  painter.drawImage(e->rect().topLeft(), image);
}