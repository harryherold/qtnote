/*
 * config.cpp
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
#include "config.hpp"
#include <QDir>
#include <QDebug>
#include <iostream>
#include <QTextStream>
#include <QStringList>

Config::Config()
  :configName("notedb.conf")
{
  homePath = QString(std::getenv("HOME")) + QString("/.note/");
  configSections["Paths"] = &Config::parsePaths;
  configSections["Editor"] = &Config::parseEditor;
}

QMap<QString,QString>& Config::getDatabases( void )
{
  return databases;
}

QMap<QString,QString>& Config::getEditor( void )
{
  return editor;
}

QString& Config::getCurrentDatabase( void )
{
  return currentDatabase;
}

bool Config::parseFile( void )
{
  QFile file(homePath + configName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;
  linecount = 0;
  QTextStream inStream(&file);
  QString lineString;

  while( ! inStream.atEnd() ) {
    lineString = inStream.readLine();
    linecount++;
    // comments and empty lines
    if( lineString.startsWith("#") || lineString.isEmpty() )
      continue;

    else if( lineString.startsWith("Section") ) {
      lineString.remove("Section");
      lineString = lineString.simplified();
      // keyword doesn't exists
      if( ! configSections.contains(lineString) ) {
        errorMesg = "Section keyword not found\n";
        return false;
      }
      // call appropriate parse function
      MemFuncGetter getter = configSections[lineString];
      if(! (this->*getter)( inStream ) ) {
        return false;
      }
    } else {
      errorMesg = "No Section found \n";
      return false;
    }
  }
  file.close();
  return true;
}

bool Config::parsePaths( QTextStream & stream )
{
  QString tmp_string;

  while( ! stream.atEnd() ) {
    tmp_string = stream.readLine();
    linecount++;
    tmp_string = tmp_string.simplified();
    // comments and empty lines
    if( tmp_string.startsWith("#") || tmp_string.isEmpty() )
      continue;
    // check there is only one assignment ( = )
    else if(tmp_string.contains("=")) {

      QStringList list = tmp_string.split("=",QString::SkipEmptyParts);
      QString key = list.at(0).simplified();
      QString value = list.at(1).simplified();

      //contains only one currentdb
      if( key.contains("currentdb") )
        currentDatabase = value;
      else
        databases[ key ] = value;

    } else if ( tmp_string.startsWith("EndSection") ) {
      break;
    } else {
      errorMesg = "No Keywords found\n";
      return false;
    }
  }
  return true;
}

bool Config::parseEditor( QTextStream & stream )
{
  QString tmp_string;

  while( ! stream.atEnd() ) {
    tmp_string = stream.readLine();
    linecount++;
    tmp_string = tmp_string.simplified();
    // comments and empty lines
    if( tmp_string.startsWith("#") || tmp_string.isEmpty() )
      continue;
    else if(tmp_string.contains("=")) {

      QStringList list = tmp_string.split("=",QString::SkipEmptyParts);
      QString key = list.at(0).simplified();
      QString value = list.at(1).simplified();

      //contains only one currentdb
      if( key.contains("path") )
        editor[ key ] = value;
      else if( key.contains("options") )
        editor[ key ] = value;
      else {
        errorMesg = "Editor keyword not found\n";
        return false;
      }

    } else if ( tmp_string.startsWith("EndSection") ) {
      break;
    } else {
      errorMesg = "Syntax error in editor section\n";
      return false;
    }
  }
  return true;
}

bool Config::writeConfigToDisk( QMap<QString,QString> & dataBases,
                                QMap<QString,QString>& edit, QString &current)
{
  QFile file( homePath + configName );

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;

  QTextStream outStream(&file);
  QList<QString> keys = dataBases.keys();

  // write paths to file
  outStream << "#Paths to the databases\n";
  outStream << "Section Paths\n";
  foreach( QString key, keys ) {
    outStream << "  " << key << "=" << dataBases[key] << "\n";
  }
  outStream << "  " << "currentdb=" << current << "\n";
  outStream << "EndSection\n";

  // write editor section
  QList<QString> edit_keys = edit.keys();
  outStream << "#editor configuration\n";
  outStream << "Section Editor\n";
  foreach( QString key, edit_keys ) {
    outStream << "  " << key << "=" << edit[key] << "\n";
  }
  outStream << "EndSection\n";
  file.close();
  return true;
}

config_t Config::checkConfig(void)
{
  QDir directory = QDir( homePath );

  if( ! directory.exists() ) {
    directory.mkdir( homePath );
  }

  if( ! QFile::exists( homePath + configName ) ) {
    QFile file( homePath + configName );
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return no_conf;
    file.close();
    return empty_conf;
  }
  if ( ! parseFile() ) {
    qDebug() << "Parsing failed in line " << linecount << "\n";
    qDebug() << errorMesg;
    return no_conf;
  }

  return load_conf;
}

