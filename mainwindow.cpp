#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  //  connect(ui->sendWidget, &SendWidget::send, ui->receiveWidget,
  //          &ReceiveWidget::receive);
  connect(ui->clearButton, &QPushButton::pressed, ui->sendWidget,
          &SendWidget::clearScreen);

  SendWorker *sendWorker = new SendWorker;
  sendWorker->moveToThread(&sendThread);

  connect(ui->sendWidget, &SendWidget::send, sendWorker,
          &SendWorker::sendPackage);

  recvWorker = new RecvWorker;
  recvWorker->moveToThread(&recvThread);

  connect(&recvThread, &QThread::started, recvWorker, &RecvWorker::loop);
  sendThread.start();
  recvThread.start();
}

MainWindow::~MainWindow() {
  recvWorker->quit();
  recvThread.quit();
  recvThread.wait();
  sendThread.quit();
  sendThread.wait();
  delete ui;
}
