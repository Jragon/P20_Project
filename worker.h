#ifndef SENDWORKER_H
#define SENDWORKER_H

#include <QObject>
#include <QThread>
#include "package.h"

class SendWorker : public QObject {
  Q_OBJECT
 public:
  explicit SendWorker(QObject *parent = nullptr);

 signals:

 public slots:
  void sendPackage(package_t pkg);

 private:
  void writeByte(quint8 byte);
};

class RecvWorker : public QObject {
  Q_OBJECT
 public:
  explicit RecvWorker(QObject *parent = nullptr);

 signals:
  void received(package_t pkg);

 public slots:
  void loop();
  void quit() { exitFlag = true; }

 private:
  quint8 readByte();
  bool exitFlag;
};

#endif  // SENDWORKER_H
