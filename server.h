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
#include "phases.h"
#include <fstream>
#include <sstream>
#include <string>

class Server : public QObject {
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
  std::list<std::string> explode(std::string input, char sep);

private Q_SLOTS:
  void onNewConnection();
  void processMsg(QString msg);
};

#endif // SERVER_H
