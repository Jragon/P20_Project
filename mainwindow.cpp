#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setupWiringPi();

  // idk need to work on the actual position of the recv widget
  // for somereason this->x() doesn't return the x pos of
  // mainwindow. Maybe it's because mainwindow has been constructed?
  QSize dSize = QApplication::desktop()->availableGeometry().size();
  int x = dSize.width() / 2 - this->width();
  int y = (dSize.height() - this->height()) / 2;

  recvWidget = new ReceiveWidget();
  recvWidget->setWindowTitle("Receive Window");
  recvWidget->resize(this->size());
  recvWidget->setStyleSheet("background-color: rgb(255, 255, 255)");
  recvWidget->move(x, y);
  recvWidget->show();

  connect(ui->clearButton, &QPushButton::pressed, ui->sendWidget,
          &SendWidget::clearScreen);
  connect(ui->colourInput,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
          ui->sendWidget, &SendWidget::penColour);
  connect(ui->sizeInput,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          ui->sendWidget, &SendWidget::penWidth);

  sendWorker = new SendWorker;
  sendWorker->moveToThread(&sendThread);

  connect(ui->sendWidget, &SendWidget::send, sendWorker,
          &SendWorker::sendPackage);

  recvWorker = new RecvWorker;
  recvWorker->moveToThread(&recvThread);

  connect(&recvThread, &QThread::started, recvWorker, &RecvWorker::loop);
  connect(recvWorker, &RecvWorker::received, recvWidget,
          &ReceiveWidget::receive);

  sendThread.start();
  recvThread.start();

  // send initial white image to set width
  //ui->sendWidget->sendImage();
}

MainWindow::~MainWindow() {
  recvWorker->quit();
  recvThread.quit();
  recvThread.wait();
  sendThread.quit();
  sendThread.wait();

  recvWidget->close();

  delete ui;
  delete recvWidget;

  QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  recvWidget->hide();
  event->accept();
}

void MainWindow::setupWiringPi() {
  wiringPiSetup();

  // send
  pinMode(package_t::txSyncOut, OUTPUT);  // tx sync  - out
  pinMode(package_t::rxSyncIn, INPUT);    // rx sync  - in
  pinMode(package_t::cmdSyncOut, OUTPUT); // cmd sync - out
  pinMode(package_t::dataOut1, OUTPUT);    // data 1    - out
  pinMode(package_t::dataOut2, OUTPUT);    // data 2    - out
  pinMode(package_t::dataOut3, OUTPUT);    // data 3    - out
  pinMode(package_t::dataOut4, OUTPUT);    // data 4    - out

  // receive
  pinMode(package_t::txSyncIn, INPUT);
  pinMode(package_t::rxSyncOut, OUTPUT);
  pinMode(package_t::cmdSyncIn, INPUT);
  pinMode(package_t::dataIn1, INPUT);
  pinMode(package_t::dataIn2, INPUT);
  pinMode(package_t::dataIn3, INPUT);
  pinMode(package_t::dataIn4, INPUT);

  digitalWrite(package_t::txSyncOut, 0);
  digitalWrite(package_t::rxSyncOut, 0);
  digitalWrite(package_t::cmdSyncOut, 0);
  digitalWrite(package_t::dataOut1, 0);
  digitalWrite(package_t::dataOut2, 0);
  digitalWrite(package_t::dataOut3, 0);
  digitalWrite(package_t::dataOut4, 0);
}
