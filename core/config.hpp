/*
 * config.hpp
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
#ifndef CONFIG_H
#define CONFIG_H
#include <vector>
#include <QString>
#include <QMap>
#include <QTextStream>
#include "util.h"


class Config
{

public:

  explicit Config();
  config_t checkConfig( void );
  void loadConfig( void );
  void insertPath( QString );
  bool parseFile( void );
  bool parsePaths( QTextStream & );
  bool parseEditor( QTextStream & );
  bool writeConfigToDisk( QMap<QString,QString>&, QMap<QString,QString>&,
                          QString&);

  QMap<QString,QString>& getDatabases( void );
  QMap<QString,QString>& getEditor( void );
  QString& getCurrentDatabase( void );

private:

  QString homePath;
  QString configName;

  typedef bool ( Config::*MemFuncGetter )( QTextStream & );
  QMap<QString,QString> databases;
  QMap<QString,QString> editor;

  QMap<QString,MemFuncGetter> configSections;

  QString editorOptions;
  QString currentDatabase;

  int linecount;
  QString errorMesg;
};

#endif /* CONFIG_H */
