#include "server.h"

Server::Server(QLabel *lbl) {
  server = new QWebSocketServer(QString("msTestServer"),
                                QWebSocketServer::NonSecureMode);

  qDebug() << "starting server";
  if (server->listen(QHostAddress::Any, 8080)) {
    connect(server, &QWebSocketServer::newConnection, this,
            &Server::onNewConnection);
  } else {
    qDebug() << "NOT ";
  }
  qDebug() << "listening for client on 8080\n";
  phases = {"start", "touch", "brightness", "end", "finished"};
  this->lbl = lbl;

  currentPhase = 0;
  lbl->setText(phases.at(currentPhase));
  lbl->update();
  lbl->repaint();
}

Server::~Server() { server->disconnect(); }

void Server::onNewConnection() {
  client = server->nextPendingConnection();

  connect(client, &QWebSocket::textMessageReceived, this, &Server::processMsg);
  connect(client, &QWebSocket::disconnected, this, [this]() {
    qDebug() << "client disconnected";
    client->deleteLater();
  });
}

// di norma:
// il server manda il numero + passato/skip/fallito del test appena concluso

void Server::processMsg(QString msg) {
  qDebug() << "received" << msg;
  qDebug() << "phase: " << phases.at(currentPhase);
  lbl->setText(phases.at(currentPhase++));
  lbl->update();
  lbl->repaint();
}

void Server::sendMsg(QString msg) {
  lbl->setText(phases.at(currentPhase));
  lbl->update();
  lbl->repaint();

  file << phases.at(currentPhase).toStdString() << " " << msg.toStdString() << std::endl;
  // può cambiare il normale ordine, se ad esempio clicco tutti i bottoni
  // currentphase va avanti e il client fa fare l'avanzamento di fase sul server

  if (currentPhase == 0) {
      file.open("/tmp/msTest/" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss").toStdString() + ".txt");
      file << "test started\n";

      serialWrite("killall msTest\n");
      serialWrite("dd if=/dev/zero of=/dev/fb0\n");
      serialWrite(
          "date +%D -s " +
          QDateTime::currentDateTime().toString("MM/dd/yyyy").toStdString() + "\n");
      serialWrite(
          "date +%T -s " +
          QDateTime::currentDateTime().toString("hh:mm:ss").toStdString() + "\n");
      serialWrite("/usr/bin/./msTest " + getServerAddress() +
                          " & \n");
      //  serialWrite("ifconfig eth0 192.168.0.1 255.255.255.0"); // from gui
  } else {
      client->sendTextMessage(msg);

      if (currentPhase == phases.size() - 2) {
          file.close();
          serialWrite("dd if=/dev/zero of=/dev/fb0\n");
      }
  }
  if (currentPhase != phases.size() - 1)
    currentPhase++;
//  else close window?
}

void Server::serialWrite(std::string cmd) {
  QByteArray writeData(cmd.c_str());
  std::cout << "writing: " << writeData.toStdString();

  QSerialPort serialPort;
  serialPort.setPortName("/dev/ttyUSB0");
  serialPort.setBaudRate(QSerialPort::Baud115200);

  qDebug() << "serial port: ";
  if (!serialPort.open(QIODevice::WriteOnly))
    qDebug() << "cannot open the device";
  const qint64 bytesWritten = serialPort.write(cmd.c_str());

  if (bytesWritten == -1)
    qDebug() << "wrote nothing";
  else if (bytesWritten != writeData.size())
    qDebug() << "cannot write all the data";
  else if (!serialPort.waitForBytesWritten(5000))
    qDebug() << "error while writing, timeout";
  else
    qDebug() << bytesWritten << " succesfully written";
  qDebug() << "\n";
}

std::string Server::getServerAddress() {
  QTcpSocket socket;
  socket.connectToHost("8.8.8.8", 53); // google DNS
  return socket.waitForConnected()
             ? socket.localAddress().toString().toStdString()
             : "ws://localhost:8080";
}
