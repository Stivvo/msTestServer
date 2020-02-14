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
  Server(QLabel *lbl);
  ~Server();
  void serialWrite(std::string);
  std::string getServerAddress();
  void sendMsg(QString msg);
  void log(int value);

private:
  QWebSocketServer *server;
  QWebSocket *client;
  std::ofstream file;

private Q_SLOTS:
  void onNewConnection();
  void processMsg(QString msg);
  std::vector<QString> phases;
  int currentPhase;
  QLabel *lbl;
};

#endif // SERVER_H
