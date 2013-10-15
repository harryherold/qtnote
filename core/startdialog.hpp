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

#include "DbNote.h"
#include "notedialog.h"
#include "util.h"
#include "categorydialog.h"

class StartDialog : public QMainWindow
{

  Q_OBJECT

public:

  StartDialog(QMainWindow *parent = 0);
  QSize getTableSize();
  QStandardItemModel * getCurrentModel( void );
  
  ~StartDialog();

public slots:
  void sig_accept();
  void sig_version();
  void sig_cancel();
  void sig_showNote();
  void sig_newNote();
  void sig_saveNote(save_t saveMode);
  void sig_closeDialog( void );
  void sig_deleteNote( void );
  void sig_show_category( void );
  void sig_show_notes( void );
  void sig_saveCategory( CategoryDialog *cat_dialog );
private:

  void createActions( void );
  void createMenus( void );
  void intialTableModel( void );
  void initialCategoryModel( void );
  QString version = "0.1";
  
  QTableView *table;
  QStandardItemModel *model;
  QStandardItemModel *model_category;
  
  DbNote *db;
  
  int colCount;
  int count_col_category = 2;
  bool note_view_active;
  
  NoteDialog *existingNote;
  NoteDialog *newNoteDialog;


  std::vector<Category> cats;
  std::vector<Note> all;

  QPushButton *exitButton;
  QPushButton *showText;
  QPushButton *deleteButton;
  QPushButton *newNote;

  QMenu *system_menu;
  QMenu *note_menu;
  QMenu *edit_menu;
  QMenu *category_menu;
  QMenu *help_menu;

  QAction *del_action;
  QAction *edit_action;
  QAction *new_action;
  QAction *exit_action;
  QAction *version_action;
  QAction *show_category_action;
  QAction *note_show_action;
  
  CategoryDialog *category_dialog;
  
  QWidget *centerWidget;
  QDialog *noteTextWidget;
  QComboBox *comboCategory;
  QLabel* noteNumber;
  QTextEdit *noteText;
  QLineEdit *noteTitle;
};
#endif // STARTDIALOG_H
