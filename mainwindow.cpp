#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);

  QLayout *col = new QVBoxLayout();
  QLayout *lblRow = new QHBoxLayout();
  QLayout *btnRow = new QHBoxLayout();

  QLabel *lblNameTest = new QLabel(this);
  lblRow->addWidget(lblNameTest);

  QLabel *lblEvent = new QLabel(this);
  lblEvent->setVisible(false);
  lblRow->addWidget(lblEvent);

  QLabel *lblNtested = new QLabel(this);
  lblNtested->setVisible(false);
  lblRow->addWidget(lblNtested);

  QPushButton *btnPrev = new QPushButton(this);
  btnPrev->setText("go back");
  connect(btnPrev, &QPushButton::released, this,
          [this]() { server->sendMsg("passed"); });
  btnRow->addWidget(btnPrev);

  QPushButton *btnPass = new QPushButton(this);
  btnPass->setText("passed");
  connect(btnPass, &QPushButton::released, this,
          [this]() { server->sendMsg("passed"); });
  btnRow->addWidget(btnPass);

  QPushButton *btnSkip = new QPushButton(this);
  btnSkip->setText("skip");
  connect(btnSkip, &QPushButton::released, this,
          [this]() { server->sendMsg("skipped"); });
  btnRow->addWidget(btnSkip);

  QPushButton *btnFail = new QPushButton(this);
  btnFail->setText("failed");
  connect(btnFail, &QPushButton::released, this,
          [this]() { server->sendMsg("failed"); });
  btnRow->addWidget(btnFail);

  col->addItem(lblRow);
  col->addItem(btnRow);

  ui->centralwidget->setLayout(col);
  this->setCentralWidget(ui->centralwidget);
  ui->centralwidget->show();

  server = new Server(lblNameTest, lblEvent, lblNtested);

  qDebug() << "starting server";
}

MainWindow::~MainWindow() { delete ui; }
