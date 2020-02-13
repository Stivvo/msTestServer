#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "server.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtCore/QObject>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  Server *server;
};
#endif // MAINWINDOW_H
