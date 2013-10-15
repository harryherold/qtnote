#include <QString>
#include <QDebug>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QList>
#include <QHeaderView>
#include <QMenuBar>
#include <QStatusBar>
#include <QModelIndexList>

#include <algorithm>
#include <functional>
#include "startdialog.hpp"
#include "category.h"
#include "note.h"
#include "date.h"
#include "util.h"


StartDialog::StartDialog( QMainWindow *parent )
  :QMainWindow(parent),note_view_active( true )
{
  centerWidget = new QWidget();
  
  QGridLayout *mainGrid = new QGridLayout;
  QVBoxLayout *topLayout = new QVBoxLayout;
  model = new QStandardItemModel(this);
  model_category = new QStandardItemModel(this);
  
  db = DbNote::getInstance();
  intialTableModel();
  initialCategoryModel();
  
  table = new QTableView(this);
  table->setModel( model );
  table->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  
  topLayout->addWidget(table);
  
  mainGrid->addLayout(topLayout,0,0);
  centerWidget->setLayout(mainGrid);
  setCentralWidget( centerWidget );
  resize( getTableSize() );
  cats = db->getAllCategories();
  
  noteTextWidget = NULL;
  category_dialog = NULL;
  
  createActions();
  createMenus();
  statusBar()->showMessage( QString("Welcome to qtnote !!") );
}

StartDialog::~StartDialog() {}

void StartDialog::createMenus()
{
  system_menu = menuBar()->addMenu(tr("&System"));
  system_menu->addAction(exit_action);
  
  edit_menu = menuBar()->addMenu(tr("&Edit"));
  edit_menu->addAction(new_action);
  edit_menu->addAction(edit_action);
  edit_menu->addAction(del_action);
  note_menu = menuBar()->addMenu(tr("&Note"));
  //~ note_menu->addAction(new_action);
  //~ note_menu->addAction(edit_action);
  //~ note_menu->addAction(del_action);
  note_menu->addAction(note_show_action);
  
  category_menu = menuBar()->addMenu(tr("&Category"));
  category_menu->addAction( show_category_action );
  
  help_menu = menuBar()->addMenu(tr("&Help"));
  help_menu->addAction( version_action );
  
  
}

void StartDialog::createActions()
{
  del_action = new QAction(tr("&delete"), this);
  del_action->setShortcuts(QKeySequence::Delete);
  del_action->setStatusTip("Delete notes");
  connect(del_action, SIGNAL(triggered()), this, SLOT(sig_deleteNote()));
  
  new_action = new QAction(tr("&create"), this);
  new_action->setShortcuts(QKeySequence::New);
  new_action->setStatusTip("Create a note");
  connect(new_action, SIGNAL(triggered()), this, SLOT(sig_newNote()));
  
  edit_action = new QAction(tr("&open"), this);
  edit_action->setShortcuts(QKeySequence::Open);
  edit_action->setStatusTip("Edit a note");
  connect(edit_action, SIGNAL(triggered()), this, SLOT(sig_showNote()));
  
  note_show_action = new QAction(tr("&show"), this);
  note_show_action->setShortcuts(QKeySequence::Forward);
  note_show_action->setStatusTip("shows the notes");
  connect(note_show_action, SIGNAL(triggered()), this, SLOT(sig_show_notes()));
  
  exit_action = new QAction(tr("&exit"), this);
  exit_action->setShortcuts(QKeySequence::Close);
  exit_action->setStatusTip("quit");
  connect(exit_action, SIGNAL(triggered()), this, SLOT(sig_cancel()));
  
  show_category_action = new QAction(tr("&show"), this);
  show_category_action->setShortcuts(QKeySequence::Back);
  show_category_action->setStatusTip("show");
  connect(show_category_action, SIGNAL(triggered()), this, SLOT(sig_show_category()));
  
  version_action = new QAction(tr("&version"), this);
  version_action->setStatusTip("Shows the version");
  connect(version_action, SIGNAL(triggered()), this, SLOT(sig_version()));
}

QSize StartDialog::getTableSize()
{
  QHeaderView *vHeader = table->verticalHeader();
  QHeaderView *hHeader = table->horizontalHeader();
  int doubleFrame = 2 * table->frameWidth();
  int wide = hHeader->length() + vHeader->width() + doubleFrame;
  int high = vHeader->length() + hHeader->height() + doubleFrame;
  return QSize(wide+180, high+100);
}
void StartDialog::intialTableModel()
{
  colCount = 4;
  all = db->getAllNotes();
  int rowCount = all.size();
  model->setColumnCount(colCount);
  model->setRowCount(rowCount);

  for( int i = 0; i < rowCount ; i++) {

    QStandardItem *key = new
    QStandardItem(QString("%1").arg(all[i].getNoteKey()));
    model->setItem(i,0,key);

    QStandardItem *datum = new
    QStandardItem(QString(all[i].getDate().getDateString().c_str()));
    model->setItem(i,1,datum);

    QStandardItem *title = new
    QStandardItem(QString(all[i].getNotetitle().c_str()));
    model->setItem(i,2,title);


    QStandardItem *cattitle = new
    QStandardItem(QString( db->getCategoryTitle(all[i].getKatKey()).c_str()));
    model->setItem(i,3,cattitle);

    //~ QStandardItem *text = new QStandardItem(true);
    //~ text->setCheckable(true);
    //~ text->setCheckState(Qt::Unchecked);
    //~ model->setItem(i,4,text);
  }
  QStringList headerLabels;
  headerLabels << "key" << "date" << "title" << "category";
  model->setHorizontalHeaderLabels(headerLabels);
  existingNote = NULL;
}

void StartDialog::initialCategoryModel()
{
    cats = db->getAllCategories();
    model_category->setColumnCount( count_col_category );
    model_category->setRowCount( cats.size() );
    
    for( unsigned int i = 0 ; i < cats.size() ; i++ ) 
    {  
        QStandardItem * key = new 
        QStandardItem( QString("%1").arg(cats[i].getKatKey()) );
        model_category->setItem( i , 0 , key );

        QStandardItem *title = new
        QStandardItem( QString( cats[i].getDesc().c_str() ) );
        model_category->setItem(i,1,title);
    }
    QStringList headerLabels;
    headerLabels << "id" << "category name";
    model_category->setHorizontalHeaderLabels(headerLabels);
}

QStandardItemModel * StartDialog::getCurrentModel( void )
{
    return ( note_view_active ) ? model : model_category;
}

void StartDialog::sig_newNote(void)
{
  
  if( note_view_active )
  {     
      if( existingNote != NULL ) 
      {
        delete existingNote;
        existingNote = NULL;
      }
      existingNote = new NoteDialog( this );

      connect( existingNote, SIGNAL(changedDialog(save_t)), this, SLOT(sig_saveNote(save_t)));

      existingNote->categoryComboBox->addItem(QString("select one"));
      std::for_each(cats.begin(),cats.end(),[&](Category& item) {
        existingNote->categoryComboBox->addItem(QString(item.getDesc().c_str()));
      });
      existingNote->saveMode = append_entr;
      existingNote->resize(500,500);
      existingNote->noteLineE->setFocus( Qt::OtherFocusReason );
      existingNote->show();
  }
  else
  {
      if( category_dialog != NULL ) 
      {
        delete category_dialog;
        category_dialog = NULL;
      }
      category_dialog = new CategoryDialog( this );
      category_dialog->show();
      connect( category_dialog, SIGNAL(sig_saveCategory(CategoryDialog *)), this, SLOT(sig_saveCategory(CategoryDialog *)));
  }
  
}

void StartDialog::sig_deleteNote(void)
{
  std::vector<int> deletedNotes;
  QMessageBox msgBox;
  
  QModelIndexList indexes = table->selectionModel()->selection().indexes();
  
  QModelIndex index = indexes.at(0);

  int row = index.row();
  
  QStandardItemModel * current_model = getCurrentModel();
  
  if( note_view_active )
  {
      if ( !db->deleteNote( current_model->item( row, 0 )->text().toInt() )) 
      {
        msgBox.setText("Can't delete item");
        msgBox.exec();
      }
  }
  else
  {
      if ( !db->deleteCategory( current_model->item( row, 0 )->text().toInt() )) 
      {
        msgBox.setText("Can't delete item");
        msgBox.exec();
      } 
  }
  current_model->removeRows( row, 1 );
  
  table->setModel( current_model );
}


void StartDialog::sig_accept()
{
  QMessageBox msgBox;
  msgBox.setText("OK Clicked!");
  msgBox.exec();
}

void StartDialog::sig_version()
{
  QMessageBox msgBox;
  msgBox.setText("Current version of qtnote: "+version);
  msgBox.exec();
}

void StartDialog::sig_show_category()
{
  table->setModel( model_category );
  note_view_active = false;
}

void StartDialog::sig_show_notes()
{
  table->setModel( model );
  note_view_active = true;
}

void StartDialog::sig_cancel()
{
  exit(0);
}


void StartDialog::sig_closeDialog(void)
{
  noteTextWidget->setVisible(false);
}


void StartDialog::sig_showNote()
{
  if( existingNote != NULL ) {
    delete existingNote;
    existingNote = NULL;
  }
  
  QModelIndexList indexes = table->selectionModel()->selection().indexes();
  
  QModelIndex index = indexes.at(0);
  
  int row = index.row(); 
  existingNote = new NoteDialog(this);
  connect( existingNote, SIGNAL(changedDialog(save_t)), this, SLOT(sig_saveNote(save_t)));
        
  existingNote->table_idx = row;
  existingNote->noteLineE->setText(QString(all[row].getNotetitle().c_str()));
  existingNote->noteTextE->setPlainText(QString(all[row].getNoteText().c_str()));
  
  for(unsigned int j = 0; j < cats.size() ; j++) {
    existingNote->categoryComboBox->addItem(QString(cats[j].getDesc().c_str()));
    if( cats[j].getKatKey() == all[row].getKatKey() ) {
        existingNote->categoryComboBox->setCurrentIndex(j);
    }
   }
   existingNote->saveMode = write_entr;
   existingNote->resize(500,500);
   existingNote->noteTextE->setFocus( Qt::OtherFocusReason );
   existingNote->show();
}

void StartDialog::sig_saveCategory( CategoryDialog *cat_dialog )
{
  int rowCount = model_category->rowCount();
  Category cat( cat_dialog->getCategoryText().toUtf8().constData() );
  
  db->saveCategory( cat , append_entr );
  
  cats.push_back( cat );
  
  QStandardItem *key  = new
                        QStandardItem(QString("%1").arg(cat.getKatKey()));
  
  QStandardItem *desc = new 
                        QStandardItem( QString(cat.getDesc().c_str() ) );
  
  model_category->setItem( rowCount , 0 ,  key);
  model_category->setItem( rowCount , 1 ,  desc);
}

void StartDialog::sig_saveNote(save_t saveMode)
{
  QMessageBox msgBox;
  int idx = existingNote->table_idx;
  QString noteText = existingNote->noteTextE->toPlainText();
  QString catText = existingNote->categoryComboBox->currentText();
  QString noteTitle = existingNote->noteLineE->text();

  if( saveMode == append_entr ) {
    all.push_back( Note( Date() ) );
    idx = all.size() - 1;
    model->setRowCount( all.size() );
    all[idx].setKatKey(1);
    QStandardItem *datum = new
    QStandardItem(QString(all[idx].getDate().getDateString().c_str()));
    model->setItem(idx,1,datum);
  }

  all[idx].setNoteText(noteText.toUtf8().constData());
  all[idx].setNoteTitle(noteTitle.toUtf8().constData());

  model->setItem(idx, 2, new QStandardItem(noteTitle));
  
  if( catText != QString(db->getCategoryTitle(all[idx].getKatKey()).c_str()) ) {
    std::for_each(cats.begin(),cats.end(),[&](Category& item) {
      if( catText == QString(item.getDesc().c_str()) ) {
        all[idx].setKatKey(item.getKatKey());
        return;
      }
    });
  }
  
  model->setItem(idx, 3, new QStandardItem(catText));
  
  if( ! db->saveEntry(all[idx], saveMode) ) {
    msgBox.setText("Sync failed !!");
    msgBox.exec();
  }
  
  if( saveMode == append_entr ) {
    existingNote->saveMode = write_entr;
    existingNote->table_idx = idx;
    
    all[idx].setNoteKey( db->getNextNoteKey() );
    
    QStandardItem *key = new
    QStandardItem(QString("%1").arg(all[idx].getNoteKey()));
    model->setItem(idx,0,key);
  }
  table->setModel(model);
}
