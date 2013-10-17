/*
 * startdialog.hpp
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
#ifndef STARTDIALOG_H
#define STARTDIALOG_H
#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QTextEdit>
#include <QLabel>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QMainWindow>
#include <QAction>

#include <DbNote.h>
#include <notedialog.h>
#include <util.h>
#include <categorydialog.h>

class StartDialog : public QMainWindow
{

    Q_OBJECT

public:

    StartDialog(QMainWindow *parent = 0);
    ~StartDialog();

    QSize                 getTableSize();
    QStandardItemModel*   getCurrentModel( void );

public slots:

    void                  slot_version();
    void                  slot_cancel();
    void                  slot_showNote();
    void                  slot_newNote();
    void                  slot_saveNote(save_t saveMode);
    void                  slot_closeDialog( void );
    void                  slot_deleteNote( void );
    void                  slot_saveCategory( CategoryDialog *cat_dialog );
    void                  slot_showCategory( void );
    void                  slot_activate_category( void );
    void                  slot_activate_note( void );

private:

    enum class View
    {
        CATEGORY,
        NOTE
    };

    int                   colCount;
    int                   count_col_category = 2;

    DbNote*               db;
    View                  activ_view;
    CategoryDialog*       category_dialog;
    NoteDialog*           note_dialog;
    std::vector<Category> categorie_vec;
    std::vector<Note>     note_vec;

    QString               version = "0.1";
    QTableView*           table;
    QStandardItemModel*   model_note;
    QStandardItemModel*   model_category;

    QMenu*                system_menu;
    QMenu*                note_menu;
    QMenu*                edit_menu;
    QMenu*                category_menu;
    QMenu*                help_menu;

    QAction*              del_action;
    QAction*              edit_action;
    QAction*              new_action;
    QAction*              exit_action;
    QAction*              version_action;
    QAction*              show_category_action;
    QAction*              note_show_action;

    QWidget*              centerWidget;
    QDialog*              noteTextWidget;
    QComboBox*            comboCategory;
    QLabel*               noteNumber;
    QTextEdit*            noteText;
    QLineEdit*            noteTitle;

    void                  createActions( void );
    void                  createMenus( void );
    void                  intialTableModel( void );
    void                  initialCategoryModel( void );
};
#endif // STARTDIALOG_H
