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
    txSyncIn = 10,
    rxSyncOut = 11,
    cmdSyncIn = 12,
    dataOut1 = 22,
    dataOut2 = 23,
    dataOut3 = 24,
    dataOut4 = 25,
    dataIn1 = 26,
    dataIn2 = 27,
    dataIn3 = 28,
    dataIn4 = 29
  };
  quint8 cmd;
  QByteArray data;
} package_t;

Q_DECLARE_METATYPE(package_t);
#endif // PACKAGE_H
