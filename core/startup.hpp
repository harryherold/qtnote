/*
 * startup.hpp
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


#ifndef STARTUP_HPP
#define STARTUP_HPP
#include <QObject>

#include <config.hpp>
#include <util.h>
#include <DbNote.h>
#include <startdialog.hpp>
#include <configdialog.hpp>


class Startup : public QObject
{
    Q_OBJECT

public:

    Startup(QObject * parent = 0);

public slots:

    void            createDatabase();

private:

    Config          config;
    DbNote*         db;
    ConfigDialog*   cdlg;

    void            reportErrorMesg( QString );
    void            start( void );
    void            createConfig( void );
};

#endif /* STARTUP_HPP */
