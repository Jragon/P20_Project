#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), rateOut(0), rateIn(0),
      sizeOut(0), sizeIn(0),
      avgRate(10),
      countOut(0), countIn(0) {
  ui->setupUi(this);

  setupWiringPi();

  //Window had issues if an image was loaded that was smaller than the current window would allow
  this->setMinimumWidth(550);
  this->setMinimumHeight(400);

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

  connect(ui->actionOpen, &QAction::triggered, ui->sendWidget,
          &SendWidget::openImg);
  connect(ui->actionSave, &QAction::triggered, ui->sendWidget,
          &SendWidget::saveImg);
  connect(ui->sendRateSlider, &QSlider::valueChanged, ui->sendWidget, &SendWidget::setSendRate);

  connect(ui->sendRateSlider, &QSlider::valueChanged, [=](const int &newValue){
      ui->sendRateValue->setText(QString::number(newValue));
  });

  connect(ui->clearButton, &QPushButton::pressed, ui->sendWidget,
          &SendWidget::clearScreen);
  connect(ui->fillButton, &QPushButton::pressed, ui->sendWidget,
          &SendWidget::fillScreen);
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

  connect(sendWorker, &SendWorker::sendSuccess, this, &MainWindow::updateSendData);
  connect(recvWorker, &RecvWorker::receiveSuccess, this, &MainWindow::updateRecvData);

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
  pinMode(package_t::dataOut5, OUTPUT);    // data 5    - out
  pinMode(package_t::dataOut6, OUTPUT);    // data 6    - out
  pinMode(package_t::dataOut7, OUTPUT);    // data 7    - out
  pinMode(package_t::dataOut8, OUTPUT);    // data 8    - out

  // receive
  pinMode(package_t::txSyncIn, INPUT);
  pinMode(package_t::rxSyncOut, OUTPUT);
  pinMode(package_t::cmdSyncIn, INPUT);
  pinMode(package_t::dataIn1, INPUT);
  pinMode(package_t::dataIn2, INPUT);
  pinMode(package_t::dataIn3, INPUT);
  pinMode(package_t::dataIn4, INPUT);
  pinMode(package_t::dataIn5, INPUT);
  pinMode(package_t::dataIn6, INPUT);
  pinMode(package_t::dataIn7, INPUT);
  pinMode(package_t::dataIn8, INPUT);

  digitalWrite(package_t::txSyncOut, 0);
  digitalWrite(package_t::rxSyncOut, 0);
  digitalWrite(package_t::cmdSyncOut, 0);
  digitalWrite(package_t::dataOut1, 0);
  digitalWrite(package_t::dataOut2, 0);
  digitalWrite(package_t::dataOut3, 0);
  digitalWrite(package_t::dataOut4, 0);
}

void MainWindow::updateSendData(int elapsed, int size){
  countOut++;
  rateOut -= rateOut / avgRate;
  rateOut += (size / (elapsed / 1000.0)) / avgRate;
  sizeOut -= sizeOut / avgRate;
  sizeOut += size / avgRate;
  ui->sendSizeLabel->setText(QString::number(sizeOut) + " bytes");
  ui->sendRateLabel->setText(QString::number((rateOut / 1000.0)) + " kBps");
}

void MainWindow::updateRecvData(int elapsed, int size){
  countIn++;
  rateIn -= rateIn / avgRate;
  rateIn += (size / (elapsed / 1000.0)) / avgRate;
  sizeIn -= sizeIn / avgRate;
  sizeIn += size / avgRate;
  ui->recvSizeLabel->setText(QString::number(sizeIn) + " bytes");
  ui->recvRateLabel->setText(QString::number((rateIn / 1000.0)) + " kBps");
}

