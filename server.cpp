#include "server.h"

Server::Server(QLabel *lbl, QLabel *lblEvent, QLabel *lblNtested)
{
    server = new QWebSocketServer(QString("msTestServer"), QWebSocketServer::NonSecureMode);

    if (server->listen(QHostAddress::Any, 8080)) {
        connect(server, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
        qDebug() << "starting server";
    } else {
        qDebug() << "NOT ";
    }
    qDebug() << "listening for client on 8080\n";
    this->lbl = lbl;
    this->lblEvent = lblEvent;
    this->lblNtested = lblNtested;

    usbTestedCount = 0;
    btnPressedCount = 0;

    lbl->setText("press a button to start");
    lbl->update();
    lbl->repaint();

    parse("../msTestServer/config.csv");
}

Server::~Server()
{
    file.close();
    server->disconnect();
}

void Server::onNewConnection()
{
    client = server->nextPendingConnection();
    qDebug() << client->errorString();

    connect(client, &QWebSocket::textMessageReceived, this, &Server::checkEnabledProces);
    connect(client, &QWebSocket::disconnected, this, [this]() {
        qDebug() << "client disconnected";
        client->close();
        client->deleteLater();
    });
}

void Server::checkEnabledSend(QString msg)
{
    sendMsg(msg);
    while (!phases.currentEnabled())
        sendMsg("skipped");
}

void Server::checkEnabledProces(QString msg)
{
    processMsg(msg);
    while (!phases.currentEnabled())
        processMsg("skipped");
}

void Server::sendMsg(QString msg)
{
    if (phases.finished())
        return;

    showLabels(false);

    // può cambiare il normale ordine, se ad esempio clicco tutti i bottoni
    // currentphase va avanti e il client fa fare l'avanzamento di fase sul server

    if (phases.isFirst()) { // la prima volta non invia nulla al client ma lo fa partire
        file.open("/tmp/msTest/"
                  + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss").toStdString()
                  + ".txt");

        serialWrite("killall msTest\n");
        serialWrite("dd if=/dev/zero of=/dev/fb0\n");
        serialWrite("date +%D -s "
                    + QDateTime::currentDateTime().toString("MM/dd/yyyy").toStdString() + "\n");
        serialWrite("date +%T -s " + QDateTime::currentDateTime().toString("hh:mm:ss").toStdString()
                    + "\n");
        serialWrite("/usr/bin/./msTest " + getServerAddress() + " & \n");
        //  serialWrite("ifconfig eth0 192.168.0.1 255.255.255.0"); // from gui
    } else {
        client->sendTextMessage(msg);
        if (phases.isLast())
            serialWrite("dd if=/dev/zero of=/dev/fb0\n");
    }
    advance(msg);
}

void Server::processMsg(QString msg)
{
    qDebug() << "received" << msg;
    qDebug() << "phase: " << phases.currentName();

    if (msg.contains("usb")) {
        lblEvent->setText(msg);
        if (msg == "usb added") {
            usbTestedCount++;
            lblNtested->setText(
                QString("tested: %1/%2").arg(usbTestedCount).arg(phases.currentNumber()));
        }
        showLabels(true);
    } else if (msg == "button pressed") {
        btnPressedCount++;
        lblEvent->setText(msg);
        lblNtested->setText(
            QString("pressed: %1/%2").arg(btnPressedCount).arg(phases.currentNumber()));
        showLabels(true);
    } else {
        showLabels(false);
        advance(msg);
    }
}

void Server::showLabels(bool visible)
{
    lblEvent->setVisible(visible);
    lblNtested->setVisible(visible);
    lblEvent->repaint();
    lblNtested->repaint();
}

void Server::advance(QString msg)
{
    if (!phases.isFirst())
        file << phases.currentName().toStdString() << " " << msg.toStdString() << std::endl;
    // the name of the test written on the log file is the one that has just finished (after pressing a button or board message)
    phases.advance();
    if (phases.finished())
        lbl->setText("finished");
    else
        lbl->setText(phases.currentName());
    // the name of the test written on the label is the one that still has to pass/fail
    lbl->repaint();
}

void Server::serialWrite(std::string cmd)
{
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

std::string Server::getServerAddress()
{
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53); // google DNS
    return socket.waitForConnected() ? socket.localAddress().toString().toStdString() : "localhost";
}

void Server::parse(QString qfilename)
{
    std::ifstream file;
    file.open(qfilename.toStdString(), std::ios::in);
    std::string line;

    while (file >> line) {
        qDebug() << "parsing: " << QString::fromStdString(line);
        phases.parseLine(line);
    }
    phases.print();
}
