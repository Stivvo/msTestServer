#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);

  QLayout *col = new QVBoxLayout();
  QLayout *btnRow = new QHBoxLayout();

  QLabel *lblNameTest = new QLabel(this);
  col->addWidget(lblNameTest);

  QPushButton *btnPass = new QPushButton(this);
  btnPass->setText("passed");
  connect(btnPass, &QPushButton::released, this,
          [this]() { server->sendMsg("pass"); });
  btnRow->addWidget(btnPass);

  QPushButton *btnSkip = new QPushButton(this);
  btnSkip->setText("skip");
  connect(btnSkip, &QPushButton::released, this,
          [this]() { server->sendMsg("skip"); });
  btnRow->addWidget(btnSkip);

  QPushButton *btnFail = new QPushButton(this);
  btnFail->setText("failed");
  connect(btnFail, &QPushButton::released, this,
          [this]() { server->sendMsg("fail"); });
  btnRow->addWidget(btnFail);

  col->addItem(btnRow);

  ui->centralwidget->setLayout(col);
  this->setCentralWidget(ui->centralwidget);
  ui->centralwidget->show();

  server = new Server(lblNameTest);
  server->serialWrite("killall msTest\n");
  server->serialWrite("dd if=/dev/zero of=/dev/fb0\n");
  server->serialWrite(
      "date +%D -s " +
      QDateTime::currentDateTime().toString("MM/dd/yyyy").toStdString() + "\n");
  server->serialWrite(
      "date +%T -s " +
      QDateTime::currentDateTime().toString("hh:mm:ss").toStdString() + "\n");
  server->serialWrite("/usr/bin/./msTest " + server->getServerAddress() +
                      " & \n");
  //  serialWrite("ifconfig eth0 192.168.0.1 255.255.255.0"); // from gui

  qDebug() << "starting server";
}

MainWindow::~MainWindow() { delete ui; }
