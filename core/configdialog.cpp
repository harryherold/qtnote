/*
 * configdialog.cpp
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
#include <configdialog.hpp>


ConfigDialog::ConfigDialog( QWidget * parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    inputPath      = new QLineEdit(this);
    inputName      = new QLineEdit(this);
    inputEditor    = new QLineEdit(this);
    inputEditorOpt = new QLineEdit(this);
    labelPath      = new QLabel("Path to save:",this);
    labelName      = new QLabel("Name:",this);
    labelEditor    = new QLabel("Editor:",this);
    labelEditorOpt = new QLabel("Editor-Options::",this);
    buttonOk       = new QPushButton("OK",this);
    buttonlayout   = new QHBoxLayout;
    mainGrid       = new QGridLayout;
    topLayout      = new QVBoxLayout;

    connect(buttonOk, SIGNAL(clicked()), this, SLOT(slot_syncDialog()));

    mainGrid->addWidget(labelPath       ,0 ,0 );
    mainGrid->addWidget(labelName       ,1 ,0 );
    mainGrid->addWidget(labelEditor     ,2 ,0 );
    mainGrid->addWidget(labelEditorOpt  ,3 ,0 );

    mainGrid->addWidget(inputPath       ,0 ,1 );
    mainGrid->addWidget(inputName       ,1 ,1 );
    mainGrid->addWidget(inputEditor     ,2 ,1 );
    mainGrid->addWidget(inputEditorOpt  ,3 ,1 );


    buttonlayout->addWidget( buttonOk );

    topLayout->addLayout( mainGrid );
    topLayout->addLayout( buttonlayout );

    setLayout( topLayout );

    resize(400,100);

}

void ConfigDialog::slot_syncDialog(void)
{
    emit changedDialog();
    setVisible( false );
}

QString ConfigDialog::getPath( void )
{
    return inputPath->text();
}

QString ConfigDialog::getName( void )
{
    return inputName->text();
}

QString ConfigDialog::getEditor( void )
{
    return inputEditor->text();
}

QString ConfigDialog::getEditorOpt( void )
{
    return inputEditorOpt->text();
}

