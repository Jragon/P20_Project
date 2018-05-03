#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "receivewidget.h"
#include "wiringPi.h"
#include "worker.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QThread>
#include <QDebug>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

signals:
  void deconstructing();

public slots:
  void updateSendData(int elapsed, int size);
  void updateRecvData(int elapsed, int size);

protected:
  void closeEvent(QCloseEvent *event);

private:
  Ui::MainWindow *ui;
  ReceiveWidget *recvWidget;
  QThread sendThread, recvThread;
  RecvWorker *recvWorker;
  SendWorker *sendWorker;
  QMenu *fileMenu;
  QAction *openAct;
  QAction *saveAct;
  double rateOut, rateIn;
  int sizeOut, sizeIn;
  int avgRate;
  int countOut, countIn;

  void setupWiringPi();
};

#endif // MAINWINDOW_H
