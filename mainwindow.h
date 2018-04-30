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

protected:
  void closeEvent(QCloseEvent *event);

private:
  Ui::MainWindow *ui;
  ReceiveWidget *recvWidget;
  QThread sendThread, recvThread;
  RecvWorker *recvWorker;
  SendWorker *sendWorker;

  void setupWiringPi();
};

#endif // MAINWINDOW_H
