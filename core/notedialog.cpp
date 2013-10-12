/*
 * notedialog.cpp
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


#include "notedialog.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

NoteDialog::NoteDialog(QWidget * parent, QString title, Qt::WindowFlags f)
  :QDialog(parent,f)
{
  gridLayout = new QGridLayout;
  buttonLayout = new QHBoxLayout;
  topLayout = new QVBoxLayout;

  categoryComboBox = new QComboBox(this);
  noteTextE = new QTextEdit;
  noteLineE = new QLineEdit;
  
  startedPid = false;
  
  topLayout->addWidget(noteLineE);
  topLayout->addWidget(categoryComboBox);
  topLayout->addWidget(noteTextE);

  gridLayout->addLayout(topLayout,0,0);

  exitButton = new QPushButton("close", this);
  syncButton = new QPushButton("sync", this);
  editButton = new QPushButton("edit", this);

  connect(exitButton, SIGNAL(clicked()), this, SLOT(slot_closeDialog()));
  connect(syncButton, SIGNAL(clicked()), this, SLOT(slot_syncDialog()));
  connect(editButton, SIGNAL(clicked()), this, SLOT(slot_editDialog()));

  buttonLayout->addWidget(exitButton);
  buttonLayout->addWidget(syncButton);
  buttonLayout->addWidget(editButton);

  gridLayout->addLayout(buttonLayout, 1, 0);
  
  installEventFilter(this);
  
  setLayout(gridLayout);

}

void NoteDialog::slot_closeDialog(void)
{
  setVisible(false);
}

void NoteDialog::slot_syncDialog(void)
{
  emit changedDialog(saveMode);
}

void NoteDialog::slot_editDialog(void)
{
  QString homePath = QString(std::getenv("HOME"));
  QFile file( homePath + "/.note_txt_tmp" );
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QTextStream outStream(&file);
  outStream << noteTextE->toPlainText();
  file.close();
  process.start("urxvt", QStringList() << "-e" << "vim" << QString( homePath + "/.note_txt_tmp" ) );
  startedPid = true;
}

bool NoteDialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
       if( ! startedPid ) {
        return true;
      }
      if( QProcess::Running == process.state() ) 
        return true;
       
       QString homePath = QString(std::getenv("HOME"));
       QFile file( homePath + "/.note_txt_tmp" );
       
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
       
       QTextStream inStream(&file);
       QString txt = inStream.readAll();
       file.remove();
       noteTextE->setText(txt);
    }
    return false;
}

NoteDialog::~NoteDialog()
{

}

