/*
 * categorydialog.cpp
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
 
#include <categorydialog.h>
#include <ui_categorydialog.h>

CategoryDialog::CategoryDialog(QWidget *parent, save_t sav_mode) :
    QDialog(parent),
    ui(new Ui::CategoryDialog),
    save_mode(sav_mode)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus( Qt::OtherFocusReason );
    connect( ui->buttonBox , SIGNAL(accepted()), this, SLOT( slot_saveCategory() ) );
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
}

void CategoryDialog::slot_saveCategory( void )
{
    emit sig_saveCategory( this );
}

QString CategoryDialog::getCategoryText( void )
{
    return ui->lineEdit->text();
}

void CategoryDialog::setCategoryDesc( QString cat_desc )
{
    ui->lineEdit->setText( cat_desc );
}
