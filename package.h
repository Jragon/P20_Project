#ifndef PACKAGE_H
#define PACKAGE_H

#include <QByteArray>

typedef struct{
  enum Commands { Clear = 0, Resize, Point, Image };
  quint8 cmd;
  QByteArray data;
} package_t ;

Q_DECLARE_METATYPE(package_t);
#endif  // PACKAGE_H
