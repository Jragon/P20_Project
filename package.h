#ifndef PACKAGE_H
#define PACKAGE_H

#include <QByteArray>
#include <QMetaType>

typedef struct {
  enum Commands { Clear = 0, Resize, Point, Image };
  enum Pins {
    txSyncOut = 0,
    rxSyncIn = 1,
    cmdSyncOut = 2,
    dataOut = 3,
    txSyncIn = 10,
    rxSyncOut = 11,
    cmdSyncIn = 12,
    dataIn = 13
  };
  quint8 cmd;
  QByteArray data;
} package_t;

Q_DECLARE_METATYPE(package_t);
#endif // PACKAGE_H
