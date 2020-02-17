#ifndef SERVER_H
#define SERVER_H

#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSerialPort>
#include <QString>
#include <QtCore/QObject>
#include <QtWebSockets/QtWebSockets>
#include <iostream>
#include <vector>
#include <fstream>

class Server : public QObject {
public:
  Server(QLabel *lbl, QLabel *lblUsb, QLabel *lblUsbTested);
  ~Server();
  void serialWrite(std::string);
  std::string getServerAddress();
  void sendMsg(QString msg);
  void log(int value);

private:
  QWebSocketServer *server;
  QWebSocket *client;
  std::ofstream file;
  bool usbRemoved;

private Q_SLOTS:
  void onNewConnection();
  void processMsg(QString msg);
  std::vector<QString> phases;
  int currentPhase;
  int usbTestedCount;
  int usbCount;
  QLabel *lbl;
  QLabel *lblUsb;
  QLabel *lblUsbTested;
};

#endif // SERVER_H
