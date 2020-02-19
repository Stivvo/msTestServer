#pragma once

#include <QCoreApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSerialPort>
#include <QtCore/QObject>
#include <QtWebSockets/QtWebSockets>

#include "phases.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QLabel *lbl, QLabel *lblUsb, QLabel *lblUsbTested);
    ~Server();

    std::string getServerAddress();

private:
    QWebSocketServer *server;
    QWebSocket *client;

    Phases phases;
    std::ofstream file;

    int usbTestedCount;
    int btnPressedCount;

    QLabel *lbl;
    QLabel *lblEvent;
    QLabel *lblNtested;

    void parse(QString filename);
    void showLabels(bool visible);

    void advance(QString msg);
    void serialWrite(std::string);

    void processMsg(QString msg);
    void sendMsg(QString msg);
    void checkEnabledSend(QString msg);

private Q_SLOTS:
    void checkEnabledProces(QString msg);
    void onNewConnection();
public Q_SLOTS:
    void passTest() { checkEnabledSend("passed"); }
    void failTest() { checkEnabledSend("failed"); }
    void skipTest() { checkEnabledSend("skipped"); }
};
