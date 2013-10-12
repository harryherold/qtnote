/*
 * startup.cpp
 * 
 * Copyright 2013 Christian Herold <harryherold@googlemail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <QMessageBox>
#include <QDialog>
#include <QMap>
#include <QDebug>
#include "startup.hpp"



Startup::Startup( QObject * parent )
:QObject(parent)
{
  db = DbNote::getInstance();
  
  config_t config_state = config.checkConfig();
  
  if( config_state == no_conf ) {
    reportErrorMesg("Your config-file is damaged or\n the permissions are wrong !");
  } else if ( config_state == load_conf ) {
    start();
  } else if( config_state == empty_conf ) {
    createConfig();
  }
  
}

void Startup::createDatabase( void )
{
  QMap<QString,QString> database;
  QMap<QString,QString> editor;
  QString current = cdlg->getName();
  QString fullPath = cdlg->getPath()+""+current+".ndb";
  database[ current ] = fullPath;
  
  if( ! cdlg->getEditor().isEmpty() )
    editor["path"] = cdlg->getEditor();
  
  if( ! cdlg->getEditorOpt().isEmpty() )
    editor["options"] = cdlg->getEditorOpt();
  
  config.writeConfigToDisk( database , editor , current );
  
  db->setDb( fullPath.toUtf8().constData() );
  db->createDB();
  StartDialog *dlg = new StartDialog();
  dlg->show();
}

void Startup::createConfig( void )
{
  QMessageBox msgBox;
  msgBox.setText("No config found !");
  msgBox.setInformativeText("Do you want to create a new database \n or would you register your existing databases?");
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Ok);
  int ret = msgBox.exec();
  if( ret == QMessageBox::Ok ) {
    cdlg = new ConfigDialog();
    connect( cdlg, SIGNAL(changedDialog()), this, SLOT(createDatabase()));
    cdlg->show();
  }
}

void Startup::start( void )
{
  QString currentdb = config.getCurrentDatabase();
  QString dbpath = (config.getDatabases())[ currentdb ] ;
  db->setDb( dbpath.toUtf8().constData() );
  StartDialog *dlg = new StartDialog();
  dlg->show();
}

void Startup::reportErrorMesg( QString text )
{
  QMessageBox msgBox;
  msgBox.setText( text );
  msgBox.exec();
  exit(0);
}


