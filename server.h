#pragma once

#include "phases.h"
#include <fstream>
#include <sstream>
#include <QCoreApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSerialPort>
#include <QtCore/QObject>
#include <QtWebSockets/QtWebSockets>

class Server : public QObject
{
public:
    Server(QLabel *lbl, QLabel *lblUsb, QLabel *lblUsbTested);
    ~Server();

    std::string getServerAddress();
    void sendMsg(QString msg);
    void checkEnabled(QString msg);

    void serialWrite(std::string);
    void advance(QString msg);
    void showLabels(bool visible);

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

private Q_SLOTS:
    void onNewConnection();
    void processMsg(QString msg);
};
