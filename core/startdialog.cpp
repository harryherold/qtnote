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

#include <startdialog.hpp>
#include <category.h>
#include <note.h>
#include <date.h>
#include <util.h>


StartDialog::StartDialog( QMainWindow *parent )
  :QMainWindow(parent),activ_view( NOTE )
{
  centerWidget = new QWidget();
  
  QGridLayout *mainGrid = new QGridLayout;
  QVBoxLayout *topLayout = new QVBoxLayout;
  model_note = new QStandardItemModel(this);
  model_category = new QStandardItemModel(this);
  
  db = DbNote::getInstance();
  intialTableModel();
  initialCategoryModel();
  
  table = new QTableView(this);
  table->setModel( model_note );
  table->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  
  topLayout->addWidget(table);
  
  mainGrid->addLayout(topLayout,0,0);
  centerWidget->setLayout(mainGrid);
  setCentralWidget( centerWidget );
  resize( getTableSize() );
  categorie_vec = db->getAllCategories();
  
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
  connect(del_action, SIGNAL(triggered()), this, SLOT(slot_deleteNote()));
  
  new_action = new QAction(tr("&create"), this);
  new_action->setShortcuts(QKeySequence::New);
  new_action->setStatusTip("Create a note");
  connect(new_action, SIGNAL(triggered()), this, SLOT(slot_newNote()));
  
  edit_action = new QAction(tr("&open"), this);
  edit_action->setShortcuts(QKeySequence::Open);
  edit_action->setStatusTip("Edit a note");
  connect(edit_action, SIGNAL(triggered()), this, SLOT(slot_showNote()));
  
  note_show_action = new QAction(tr("&show"), this);
  note_show_action->setShortcuts(QKeySequence::Forward);
  note_show_action->setStatusTip("shows the notes");
  connect(note_show_action, SIGNAL(triggered()), this, SLOT(slot_activate_note()));
  
  exit_action = new QAction(tr("&exit"), this);
  exit_action->setShortcuts(QKeySequence::Close);
  exit_action->setStatusTip("quit");
  connect(exit_action, SIGNAL(triggered()), this, SLOT(slot_cancel()));
  
  show_category_action = new QAction(tr("&show"), this);
  show_category_action->setShortcuts(QKeySequence::Back);
  show_category_action->setStatusTip("show");
  connect(show_category_action, SIGNAL(triggered()), this, SLOT(slot_activate_category()));
  
  version_action = new QAction(tr("&version"), this);
  version_action->setStatusTip("Shows the version");
  connect(version_action, SIGNAL(triggered()), this, SLOT(slot_version()));
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
  note_vec = db->getAllNotes();
  int rowCount = note_vec.size();
  model_note->setColumnCount(colCount);
  model_note->setRowCount(rowCount);

  for( int i = 0; i < rowCount ; i++) {

    QStandardItem *key = new
    QStandardItem(QString("%1").arg(note_vec[i].getNoteKey()));
    model_note->setItem(i,0,key);

    QStandardItem *datum = new
    QStandardItem(QString(note_vec[i].getDate().getDateString().c_str()));
    model_note->setItem(i,1,datum);

    QStandardItem *title = new
    QStandardItem(QString(note_vec[i].getNotetitle().c_str()));
    model_note->setItem(i,2,title);


    QStandardItem *cattitle = new
    QStandardItem(QString( db->getCategoryTitle(note_vec[i].getKatKey()).c_str()));
    model_note->setItem(i,3,cattitle);
  }
  QStringList headerLabels;
  headerLabels << "key" << "date" << "title" << "category";
  model_note->setHorizontalHeaderLabels(headerLabels);
  note_dialog = NULL;
  
}

void StartDialog::initialCategoryModel()
{
    categorie_vec = db->getAllCategories();
    model_category->setColumnCount( count_col_category );
    model_category->setRowCount( categorie_vec.size() );
    
    for( unsigned int i = 0 ; i < categorie_vec.size() ; i++ ) 
    {  
        QStandardItem * key = new 
        QStandardItem( QString("%1").arg(categorie_vec[i].getKatKey()) );
        model_category->setItem( i , 0 , key );

        QStandardItem *title = new
        QStandardItem( QString( categorie_vec[i].getDesc().c_str() ) );
        model_category->setItem(i,1,title);
    }
    QStringList headerLabels;
    headerLabels << "id" << "category name";
    model_category->setHorizontalHeaderLabels(headerLabels);
}

QStandardItemModel * StartDialog::getCurrentModel( void )
{
    return ( activ_view == View::NOTE ) ? model_note : model_category;
}

void StartDialog::slot_newNote(void)
{
  
  if( activ_view == View::NOTE )
  {     
      if( note_dialog != NULL ) 
      {
        delete note_dialog;
        note_dialog = NULL;
      }
      note_dialog = new NoteDialog( this );

      connect( note_dialog, SIGNAL(changedDialog(save_t)), this, SLOT(slot_saveNote(save_t)));

      note_dialog->categoryComboBox->addItem(QString("select one"));
      std::for_each(categorie_vec.begin(),categorie_vec.end(),[&](Category& item) {
        note_dialog->categoryComboBox->addItem(QString(item.getDesc().c_str()));
      });
      note_dialog->saveMode = append_entr;
      note_dialog->resize(500,500);
      note_dialog->noteLineE->setFocus( Qt::OtherFocusReason );
      note_dialog->show();
  }
  else
  {
      if( category_dialog != NULL ) 
      {
        delete category_dialog;
        category_dialog = NULL;
      }
      category_dialog = new CategoryDialog( this, write_entr );
      category_dialog->show();
      connect( category_dialog, SIGNAL(sig_saveCategory(CategoryDialog *)), this, SLOT(slot_saveCategory(CategoryDialog *)));
  }
  
}

void StartDialog::slot_deleteNote(void)
{
  std::vector<int> deletedNotes;
  QMessageBox msgBox;
  
  QModelIndexList indexes = table->selectionModel()->selection().indexes();
  
  QModelIndex index = indexes.at(0);

  int row = index.row();
  
  QStandardItemModel * current_model = getCurrentModel();
  
  if( activ_view == View::NOTE )
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

void StartDialog::slot_version()
{
  QMessageBox msgBox;
  msgBox.setText("Current version of qtnote: "+version);
  msgBox.exec();
}

void StartDialog::activate_category_model()
{
  table->setModel( model_category );
  connect(edit_action, SIGNAL(triggered()), this, SLOT(slot_showCategory()));
  activ_view = CATEGORY;
}

void StartDialog::activate_note_model()
{
  table->setModel( model_note );
  connect(edit_action, SIGNAL(triggered()), this, SLOT(slot_showNote()));
  activ_view = NOTE;
}

void StartDialog::slot_cancel()
{
  exit(0);
}


void StartDialog::slot_closeDialog(void)
{
  noteTextWidget->setVisible(false);
}

void StartDialog::slot_showCategory( void )
{
  QModelIndexList indexes = table->selectionModel()->selection().indexes();
  
  QModelIndex index = indexes.at(0);
  
  int row = index.row(); 
  
  if( category_dialog != NULL ) 
  {
    delete category_dialog;
    category_dialog = NULL;
  }
  
  category_dialog = new CategoryDialog( this, append_entr );
  
  category_dialog->setCategoryDesc( model_category->item( row, 1 )->text() );
  
  category_dialog->show();
}

void StartDialog::slot_showNote()
{
  if( note_dialog != NULL ) 
  {
    delete note_dialog;
    note_dialog = NULL;
  }
  
  QModelIndexList indexes = table->selectionModel()->selection().indexes();
  
  QModelIndex index = indexes.at(0);
  
  int row = index.row(); 
  note_dialog = new NoteDialog(this);
  connect( note_dialog, SIGNAL(changedDialog(save_t)), this, SLOT(slot_saveNote(save_t)));
        
  note_dialog->table_idx = row;
  note_dialog->noteLineE->setText(QString(note_vec[row].getNotetitle().c_str()));
  note_dialog->noteTextE->setPlainText(QString(note_vec[row].getNoteText().c_str()));
  
  for(unsigned int j = 0; j < categorie_vec.size() ; j++) 
  {
    note_dialog->categoryComboBox->addItem(QString(categorie_vec[j].getDesc().c_str()));
    if( categorie_vec[j].getKatKey() == note_vec[row].getKatKey() ) 
    {
        note_dialog->categoryComboBox->setCurrentIndex(j);
    }
  }
  note_dialog->saveMode = write_entr;
  note_dialog->resize(500,500);
  note_dialog->noteTextE->setFocus( Qt::OtherFocusReason );
  note_dialog->show();
}

void StartDialog::slot_saveCategory( CategoryDialog *cat_dialog )
{
  int rowCount = model_category->rowCount();
  Category cat( cat_dialog->getCategoryText().toUtf8().constData() );
  
  db->saveCategory( cat , append_entr );
  
  categorie_vec.push_back( cat );
  
  QStandardItem *key  = new
                        QStandardItem(QString("%1").arg(cat.getKatKey()));
  
  QStandardItem *desc = new 
                        QStandardItem( QString(cat.getDesc().c_str() ) );
  
  model_category->setItem( rowCount , 0 ,  key);
  model_category->setItem( rowCount , 1 ,  desc);
}

void StartDialog::slot_saveNote(save_t saveMode)
{
  QMessageBox msgBox;
  int idx = note_dialog->table_idx;
  QString noteText = note_dialog->noteTextE->toPlainText();
  QString catText = note_dialog->categoryComboBox->currentText();
  QString noteTitle = note_dialog->noteLineE->text();

  if( saveMode == append_entr ) {
    note_vec.push_back( Note( Date() ) );
    idx = note_vec.size() - 1;
    model_note->setRowCount( note_vec.size() );
    note_vec[idx].setKatKey(1);
    QStandardItem *datum = new
    QStandardItem(QString(note_vec[idx].getDate().getDateString().c_str()));
    model_note->setItem(idx,1,datum);
  }

  note_vec[idx].setNoteText(noteText.toUtf8().constData());
  note_vec[idx].setNoteTitle(noteTitle.toUtf8().constData());

  model_note->setItem(idx, 2, new QStandardItem(noteTitle));
  
  if( catText != QString(db->getCategoryTitle(note_vec[idx].getKatKey()).c_str()) ) {
    std::for_each(categorie_vec.begin(),categorie_vec.end(),[&](Category& item) {
      if( catText == QString(item.getDesc().c_str()) ) {
        note_vec[idx].setKatKey(item.getKatKey());
        return;
      }
    });
  }
  
  model_note->setItem(idx, 3, new QStandardItem(catText));
  
  if( ! db->saveEntry(note_vec[idx], saveMode) ) {
    msgBox.setText("Sync failed !!");
    msgBox.exec();
  }
  
  if( saveMode == append_entr ) {
    note_dialog->saveMode = write_entr;
    note_dialog->table_idx = idx;
    
    note_vec[idx].setNoteKey( db->getNextNoteKey() );
    
    QStandardItem *key = new
    QStandardItem(QString("%1").arg(note_vec[idx].getNoteKey()));
    model_note->setItem(idx,0,key);
  }
  table->setModel(model_note);
}
