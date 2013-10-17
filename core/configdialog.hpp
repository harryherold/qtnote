/*
 * configdialog.hpp
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
#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:

    ConfigDialog( QWidget * parent = 0, Qt::WindowFlags f = 0 );
    QString         getPath( void );
    QString         getName( void );
    QString         getEditor( void );
    QString         getEditorOpt( void );

public slots:

    void            slot_syncDialog(void);

signals:

    void            changedDialog( void );

private:

    QLineEdit*      inputPath;
    QLineEdit*      inputName;
    QLineEdit*      inputEditor;
    QLineEdit*      inputEditorOpt;
    QLabel*         labelPath;
    QLabel*         labelName;
    QLabel*         labelEditor;
    QLabel*         labelEditorOpt;
    QHBoxLayout*    buttonlayout;
    QGridLayout*    mainGrid;
    QVBoxLayout*    topLayout;
    QPushButton*    buttonOk;
};

#endif /* CONFIGDIALOG_HPP */
