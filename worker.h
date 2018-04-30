#ifndef SENDWORKER_H
#define SENDWORKER_H

#include "package.h"
#include <QObject>
#include <QThread>
#include <wiringPi.h>

class SendWorker : public QObject {
  Q_OBJECT
public:
  explicit SendWorker(QObject *parent = 0);

signals:

public slots:
  void sendPackage(package_t pkg);

private:
  void writeSimByte(quint8 byte);
  void writeByte(quint8 byte);
};

class RecvWorker : public QObject {
  Q_OBJECT
public:
  explicit RecvWorker(QObject *parent = 0);

signals:
  void received(package_t pkg);

public slots:
  void loop();
  void quit() { exitFlag = true; }

private:
  quint8 readSimByte();
  quint8 readByte();
  bool exitFlag;
};

#endif // SENDWORKER_H
