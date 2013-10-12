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
#include <QDebug>
#include <QtGui/QApplication>

#include "config.hpp"
#include "../util.h"

int main(int argc, char *argv[])
{
  Config c;
  config_t state = c.checkConfig();
  qDebug() << "conf " << state;
  return 0;
}
