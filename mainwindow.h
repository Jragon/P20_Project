#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
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

 private:
  Ui::MainWindow *ui;
  QThread sendThread, recvThread;
  RecvWorker *recvWorker;
};

#endif  // MAINWINDOW_H
