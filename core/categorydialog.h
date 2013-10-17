/*
 * categorydialog.h
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
#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QString>
#include <QDialog>

#include <util.h>

namespace Ui
{
class CategoryDialog;
}

class CategoryDialog : public QDialog
{
    Q_OBJECT

public:

    save_t save_mode;

    explicit CategoryDialog(QWidget *parent = 0);
    ~CategoryDialog();

    QString getCategoryText(void);
    void setCategoryDesc( QString cat_desc );

public slots:
    void slot_saveCategory( void );

signals:
    void sig_saveCategory( CategoryDialog *cat_dialog );
private:
    Ui::CategoryDialog *ui;
};

#endif // CATEGORYDIALOG_H
