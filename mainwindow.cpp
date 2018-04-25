#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->sendWidget, &SendWidget::send, ui->receiveWidget,
          &ReceiveWidget::receive);
}

MainWindow::~MainWindow() { delete ui; }
