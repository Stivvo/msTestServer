#pragma once

#include "phases.h"
#include <fstream>
#include <sstream>
#include <string>
#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSerialPort>
#include <QString>
#include <QtCore/QObject>
#include <QtWebSockets/QtWebSockets>

class Server : public QObject
{
public:
    Server(QLabel *lbl, QLabel *lblUsb, QLabel *lblUsbTested);
    ~Server();

    std::string getServerAddress();
    void sendMsg(QString msg);

    void serialWrite(std::string);
    void advance(QString msg);
    void showLabels(bool visible);

private:
    QWebSocketServer *server;
    QWebSocket *client;

    Phases phases;
    std::ofstream file;

    int usbTestedCount;
    int usbCount;

    int btnPressedCount;
    int btnCount;

    QLabel *lbl;
    QLabel *lblEvent;
    QLabel *lblNtested;

    void parse(QString filename);

private Q_SLOTS:
    void onNewConnection();
    void processMsg(QString msg);
};
