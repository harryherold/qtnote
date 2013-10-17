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
