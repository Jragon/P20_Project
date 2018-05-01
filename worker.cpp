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
  QElapsedTimer timer;
  timer.start();

  digitalWrite(package_t::cmdSyncOut, 1);
  writeByte(pkg.cmd);
  digitalWrite(package_t::cmdSyncOut, 0);

  writeByte(pkg.data.size() >> 8);
  writeByte(pkg.data.size());
  for (int i = 0; i < pkg.data.size(); i++) {
    writeByte(pkg.data.at(i));
  }

  QByteArray hash = QCryptographicHash::hash(pkg.data, QCryptographicHash::Md5);
  for (int i = 0; i < hash.size(); i++) {
    writeByte(hash.at(i));
  }

  int elapsed = timer.elapsed();
  qDebug() << "PKG OUT: " << pkg.data.size() << "bytes in " << elapsed/1000.0
           << "s | avg: " << (pkg.data.size())/(elapsed/1000.0) << "Bytes per second";
}

void SendWorker::writeByte(quint8 byte) {
  for (int i = 0; i < 8; i++) {
    // qDebug() << "Write: Start Write Bit";

    // wait for rxSync to go low
    // qDebug() << "Write: wait for rxSync to go low";

    while (digitalRead(package_t::rxSyncIn))
      ;

    // put bit on line
    // qDebug() << "Write: Put bit on line";
    digitalWrite(package_t::dataOut, byte & 0x80);
    byte <<= 1;

    // qDebug() << "Write: Put txSync high";
    digitalWrite(package_t::txSyncOut, 1);

    // wait for rxSync to go high
    // qDebug() << "Write: Wait rxsync low";
    while (!digitalRead(package_t::rxSyncIn))
      ;

    // qDebug() << "Write: txSync low";
    digitalWrite(package_t::txSyncOut, 0);

    // qDebug() << "Write: End Write Bit";
  }
}

void SendWorker::writeSimByte(quint8 byte) {
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
  digitalWrite(package_t::rxSyncOut, 0);

  while (!exitFlag) {
    if (digitalRead(package_t::cmdSyncIn)) {
      package_t pkg;
      QElapsedTimer timer;
      timer.start();

      pkg.cmd = readByte();
      int size = readByte() << 8;
      size |= readByte();
      for (int i = 0; i < size; i++) {
        pkg.data.append(readByte());
      }

      QByteArray localHash = QCryptographicHash::hash(pkg.data, QCryptographicHash::Md5);
      QByteArray remoteHash;

      // receive 32 byte md5 checksum
      for (int i = 0; i < localHash.size(); i++) {
        remoteHash.append(readByte());
      }

      if (localHash != remoteHash)
          qDebug() << "Checksums do not match";

      emit received(pkg);

      int elapsed = timer.elapsed();
      qDebug() << "PKG IN: " << pkg.data.size() << "bytes in " << elapsed/1000.0
               << "s | avg: " << (pkg.data.size())/(elapsed/1000.0) << "Bytes per second";
    }
  }
}

quint8 RecvWorker::readSimByte() {
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
    // qDebug() << "Read [" << i << "]: read data";
    data |= (rxSim.data << (7 - i));
    // qDebug() << "Read [" << i << "]: set rxSync High";
    rxSim.rxSync = 1;
    // qDebug() << "Read [" << i << "]: wait txSync low";
    while (rxSim.txSync)
      ;
    // qDebug() << "Read [" << i << "]: rxSync low";
    rxSim.rxSync = 0;
  }
  return data;
}

quint8 RecvWorker::readByte() {
  // qDebug() << "Read: Start read byte";
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
    // qDebug() << "Read [" << i << "]: start";
    // qDebug() << "Read [" << i << "]: wait txSync go high";
    while (!digitalRead(package_t::txSyncIn))
      ;
    // qDebug() << "Read [" << i << "]: read data";
    data |= (digitalRead(package_t::dataIn) << (7 - i));
    // qDebug() << "Read [" << i << "]: set rxSync High";
    digitalWrite(package_t::rxSyncOut, 1);
    // qDebug() << "Read [" << i << "]: wait txSync low";
    while (digitalRead(package_t::txSyncIn))
      ;
    // qDebug() << "Read [" << i << "]: rxSync low";
    digitalWrite(package_t::rxSyncOut, 0);
    // qDebug() << "Read [" << i << "]: end";
  }

  return data;
}
