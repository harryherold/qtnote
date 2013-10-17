/*
 * notedialog.h
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


#ifndef NOTEDIALOG_H
#define NOTEDIALOG_H
#include <QDialog>
#include <QWidget>
#include <QString>
#include <QShortcut>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProcess>

#include <QComboBox>
#include <QLabel>
#include <QEvent>
#include <QTextEdit>
#include <QLineEdit>

#include <util.h>

class NoteDialog: public QDialog
{
    Q_OBJECT

public:

    NoteDialog(QWidget * parent = 0, QString title = "", Qt::WindowFlags f = 0);
    virtual ~NoteDialog();

    int               table_idx;
    QComboBox*        categoryComboBox;
    QTextEdit*        noteTextE;
    QLineEdit*        noteLineE;
    save_t            saveMode;

    bool              eventFilter(QObject *object, QEvent *event);

public slots:

    void              slot_closeDialog(void);
    void              slot_syncDialog(void);
    void              slot_editDialog(void);

signals:

    void              changedDialog(save_t saveMode);

private:

    QPushButton*      exitButton;
    QPushButton*      syncButton;
    QPushButton*      editButton;

    QProcess          process;
    bool              startedPid;
    QGridLayout*      gridLayout;
    QHBoxLayout*      buttonLayout;
    QVBoxLayout*      topLayout;
    QShortcut*        shortcut_save;

};

#endif /* NOTEDIALOG_H */
