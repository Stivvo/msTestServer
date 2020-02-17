#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);

  QLayout *col = new QVBoxLayout();
  QLayout *btnRow = new QHBoxLayout();

  QLabel *lblNameTest = new QLabel(this);
  col->addWidget(lblNameTest);

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

  col->addItem(btnRow);

  ui->centralwidget->setLayout(col);
  this->setCentralWidget(ui->centralwidget);
  ui->centralwidget->show();

  server = new Server(lblNameTest);

  qDebug() << "starting server";
}

MainWindow::~MainWindow() { delete ui; }
