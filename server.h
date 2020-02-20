#pragma once

#include <vector>
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
    void parse(const QString &filename);

private:
    std::unique_ptr<QWebSocketServer> server;
    std::unique_ptr<QWebSocket> client;

    Phases phases;
    std::ofstream file;

    int usbTestedCount;
    int btnPressedCount;

    std::unique_ptr<QLabel> lbl;
    std::unique_ptr<QLabel> lblEvent;
    std::unique_ptr<QLabel> lblNtested;

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
