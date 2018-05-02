#ifndef PACKAGE_H
#define PACKAGE_H

#include <QByteArray>
#include <QMetaType>

typedef struct {
  enum Commands { Clear = 0, Resize, Point, Image };
  enum Pins {
    txSyncOut = 8,
    rxSyncIn = 9,
    cmdSyncOut = 7,
    txSyncIn = 0,
    rxSyncOut = 2,
    cmdSyncIn = 3,
    dataOut1 = 22,
    dataOut2 = 23,
    dataOut3 = 24,
    dataOut4 = 25,
    dataOut5 = 12,
    dataOut6 = 13,
    dataOut7 = 14,
    dataOut8 = 30,
    dataIn1 = 26,
    dataIn2 = 27,
    dataIn3 = 28,
    dataIn4 = 29,
    dataIn5 = 6,
    dataIn6 = 10,
    dataIn7 = 11,
    dataIn8 = 31
  };
  quint8 cmd;
  QByteArray data;
} package_t;

Q_DECLARE_METATYPE(package_t);
#endif // PACKAGE_H
