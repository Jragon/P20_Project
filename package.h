#ifndef PACKAGE_H
#define PACKAGE_H

#include <QByteArray>

struct package_t {
  enum Commands { Clear = 0, Resize, Point, Image };
  int cmd;
  QByteArray data;
};

#endif  // PACKAGE_H
