#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QThread>
#include "receivewidget.h"
#include "worker.h"

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
};

#endif  // MAINWINDOW_H
