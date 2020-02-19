#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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
    btnRow->addWidget(btnPrev);

    QPushButton *btnPass = new QPushButton(this);
    btnPass->setText("passed");
    btnRow->addWidget(btnPass);

    QPushButton *btnSkip = new QPushButton(this);
    btnSkip->setText("skip");
    btnRow->addWidget(btnSkip);

    QPushButton *btnFail = new QPushButton(this);
    btnFail->setText("failed");
    btnRow->addWidget(btnFail);

    col->addItem(lblRow);
    col->addItem(btnRow);

    ui->centralwidget->setLayout(col);
    this->setCentralWidget(ui->centralwidget);
    ui->centralwidget->show();

    server = new Server(lblNameTest, lblEvent, lblNtested);
    connect(btnFail, &QPushButton::released, server, &Server::failTest);
    connect(btnSkip, &QPushButton::released, server, &Server::skipTest);
    connect(btnPass, &QPushButton::released, server, &Server::passTest);
    connect(btnPrev, &QPushButton::released, server, &Server::passTest);

    qDebug() << "starting server";
}

MainWindow::~MainWindow()
{
    delete ui;
}
