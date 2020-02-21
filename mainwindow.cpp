#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    start = true;

    // layouts
    QLayout *col = new QVBoxLayout();
    QLayout *lblRow = new QHBoxLayout();
    QLayout *btnRow = new QHBoxLayout();
    QLayout *inputRow = new QHBoxLayout();

    // labels
    QLabel *lblNameTest = new QLabel(this);
    lblRow->addWidget(lblNameTest);

    QLabel *lblEvent = new QLabel(this);
    lblEvent->setVisible(false);
    lblRow->addWidget(lblEvent);

    QLabel *lblNtested = new QLabel(this);
    lblNtested->setVisible(false);
    lblRow->addWidget(lblNtested);

    // edit
    QLineEdit *editIp = new QLineEdit();
    inputRow->addWidget(editIp);

    QLineEdit *editPort = new QLineEdit();
    inputRow->addWidget(editPort);

    QComboBox *comboBaud = new QComboBox();
    comboBaud->addItems(
        {"nessuno", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"});
    inputRow->addWidget(comboBaud);

    // buttons
    QPushButton *btnStart = new QPushButton(this);
    btnStart->setText("start test");
    btnRow->addWidget(btnStart);

    QPushButton *btnPass = new QPushButton(this);
    btnPass->setText("passed");
    btnPass->setVisible(false);
    btnRow->addWidget(btnPass);

    QPushButton *btnSkip = new QPushButton(this);
    btnSkip->setText("skip");
    btnSkip->setVisible(false);
    btnRow->addWidget(btnSkip);

    QPushButton *btnFail = new QPushButton(this);
    btnFail->setText("failed");
    btnFail->setVisible(false);
    btnRow->addWidget(btnFail);

    col->addItem(inputRow);
    col->addItem(lblRow);
    col->addItem(btnRow);

    ui->centralwidget->setLayout(col);
    this->setCentralWidget(ui->centralwidget);
    ui->centralwidget->show();

    server.reset(new Server(lblNameTest, lblEvent, lblNtested));
    connect(btnFail, &QPushButton::released, server.get(), &Server::failTest);
    connect(btnSkip, &QPushButton::released, server.get(), &Server::skipTest);
    connect(btnPass, &QPushButton::released, server.get(), &Server::passTest);

    QFileDialog *configDialog = new QFileDialog(this, Qt::Dialog);
    configDialog->setFilter(QDir::Files);
    server->parse(
        configDialog->getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("csv (*.csv)")));

    connect(btnStart, &QPushButton::released, this, [=]() {
        if (start) {
            server->openSerialPort(editPort->text(),
                                   comboBaud->currentText() == "nessuno"
                                       ? -1
                                       : comboBaud->currentText().toInt(),
                                   editIp->text().toStdString());
            editPort->setVisible(false);
            editPort->repaint();
            comboBaud->setVisible(false);
            comboBaud->repaint();
            editIp->setVisible(false);
            editIp->repaint();
            btnStart->setVisible(false);
            btnStart->repaint();

            btnPass->setVisible(true);
            btnPass->repaint();
            btnFail->setVisible(true);
            btnFail->repaint();
            btnSkip->setVisible(true);
            btnSkip->repaint();

            start = false;
            qDebug() << "starting server";
        }
        server->passTest();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
