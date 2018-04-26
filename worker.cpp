#include "worker.h"
#include <QDebug>

volatile struct {
  bool txSync;
  bool rxSync;
  bool cmdSync;
  bool data;
} rxSim;

SendWorker::SendWorker(QObject *parent) : QObject(parent) {}

void SendWorker::sendPackage(package_t pkg) {
  QThread::currentThread()->usleep(500);
  rxSim.cmdSync = 1;
  writeByte(pkg.cmd);
  rxSim.cmdSync = 0;
  writeByte(pkg.data.size() >> 8);
  writeByte(pkg.data.size());
  for (int i = 0; i < pkg.data.size(); i++) {
    writeByte(pkg.data.at(i));
  }
}

void SendWorker::writeByte(quint8 byte) {
  for (int i = 0; i < 8; i++) {
    // wait for rxSync to go low
    while (rxSim.rxSync)
      ;

    // put bit on line
    rxSim.data = byte & 0x80;
    byte <<= 1;

    rxSim.txSync = 1;

    // wait for rxSync to go high
    while (!rxSim.rxSync)
      ;

    rxSim.txSync = 0;
  }
}

RecvWorker::RecvWorker(QObject *parent) : QObject(parent), exitFlag(false) {}

void RecvWorker::loop() {
  rxSim.rxSync = false;
  rxSim.txSync = false;

  while (!exitFlag) {
    if (rxSim.cmdSync) {
      package_t pkg;

      pkg.cmd = readByte();

      int size = readByte() << 8;
      size |= readByte();
      for (int i = 0; i < size; i++) {
        pkg.data.append(readByte());
      }

      emit received(pkg);
    }
  }
}

quint8 RecvWorker::readByte() {
  quint8 data = 0;
  /*
  for 8 bits:
    wait txSync high
    read bit
    rxSync high       // transmit uses rxSync to send next bit
    wait txSync low   // when rxSync goes high next bit will be sent
    rxSync low
  */
  for (int i = 0; i < 8; i++) {
    while (!rxSim.txSync)
      ;
    data |= (rxSim.data << (7 - i));
    rxSim.rxSync = 1;
    while (rxSim.txSync)
      ;
    rxSim.rxSync = 0;
  }
  return data;
}
