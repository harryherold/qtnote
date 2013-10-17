/*
 * File:   main.cpp
 * Author: harry
 *
 * Created on May 16, 2011, 8:54 PM
 */
#include <QString>
#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QtGui/QApplication>

#include <startdialog.hpp>
#include <startup.hpp>
#include <notedialog.h>

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);
  Startup s;
  return app.exec();
}
