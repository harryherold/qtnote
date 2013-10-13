#include "DbNote.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sqlite3.h>
DbNote* DbNote::singleton = 0;
struct SQLError: public runtime_error {
  SQLError(const string &msg) :
    runtime_error(string("SQL-ERROR : ") + msg) {

  }
};

void DbNote::setDb(string fileName)
{
  file = fileName;
}

DbNote* DbNote::getInstance()
{
  if (singleton == NULL)
    singleton = new DbNote();
  return singleton;
}

DbNote::~DbNote()
{
  if (singleton != NULL)
    delete singleton;
}

DbNote::DbNote()
{
}

bool DbNote::openDB()
{
  if (sqlite3_open(file.c_str(), &db) != SQLITE_OK) {
    const char * error = sqlite3_errmsg(db);
    sqlite3_close(db);
    throw SQLError(error);
    return false;
  }
  return true;
}

void DbNote::closeDB()
{
  sqlite3_close(db);
}

vector<Note>& DbNote::getAllNotes()
{
  if( ! selectNotes("") ) {
    std::cout << "Can't find notes" <<std::endl;
    return notes;
  }
  return notes;
}

vector<Category>&  DbNote::getAllCategories()
{
  if( ! selectCategories("") ) {
    std::cout << "Can't find Categories" << std::endl;
    return categories;
  }
  return categories;
}

bool DbNote::selectNotes(string where = "")
{
  sqlite3_stmt *statement;
  string all = "";
  string s = "";
  string sqlStmnt = "select NoteKey, date, NoteText, NoteTitle, "
                    "KatKey, NoteRef from NoteTable ";
  if ((where.length() > 0)) {
    sqlStmnt += where;
  }
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, sqlStmnt.c_str(), sqlStmnt.length(),
                               &statement, 0);
  if (req == SQLITE_ERROR) {
    throw SQLError("Can't select data of the given table");
    return false;
  }
  sqlite3_column_count(statement);
  char * str_time = NULL;
  int i = 0;
  do {
    req = sqlite3_step(statement);
    Note n = Note();
    if (req == SQLITE_ROW) {
      n.setNoteKey((int) sqlite3_column_int(statement, 0));
      str_time = (char *) sqlite3_column_text(statement, 1);
      if (str_time != NULL) {
        Date d = extractDate(str_time);
        n.setDate(d);
      } else {
        n.setDate(Date(0, 0, 0, 0, 0));
      }

      char * str = (char *) sqlite3_column_text(statement, 2);
      if (str != NULL)
        n.setNoteText((char *) sqlite3_column_text(statement, 2));

      str = (char *) sqlite3_column_text(statement, 3);
      if (str != NULL)
        n.setNoteTitle((char *) sqlite3_column_text(statement, 3));

      n.setKatKey((int) sqlite3_column_int(statement, 4));
      n.setNoteRef((int) sqlite3_column_int(statement, 5));
      str_time = NULL;
      notes.push_back(n);
    }
    i++;
  } while (req == SQLITE_ROW);
  sqlite3_finalize(statement);
  sqlite3_close(db);
  return true;
}

bool DbNote::selectCategories(string where = "")
{
  sqlite3_stmt *statement;
  clearCategoryCache();
  string stmnt = "select KatKey,KatDesc from KatTable ";
  if (where.length() > 0)
    stmnt += where;
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), stmnt.length(), &statement,
                               0);
  if (req == SQLITE_ERROR) {
    throw SQLError("Can't select data of the given table");
    return false;
  }
  sqlite3_column_count(statement);

  do {
    req = sqlite3_step(statement);
    Category cat = Category();
    if (req == SQLITE_ROW) {
      cat.setKatKey((int) sqlite3_column_int(statement, 0));
      char * str = (char *) sqlite3_column_text(statement, 1);
      if (str != NULL)
        cat.setKatDesc(str);
      categories.push_back(cat);
    }
    //std::cout << cat.getDesc() << "\n";
  } while (req == SQLITE_ROW);
  sqlite3_finalize(statement);
  sqlite3_close(db);
  return true;
}

Date  DbNote::extractDate(char * str)
{
  tm tt;
  const char * format = "%Y-%m-%d%n%H:%M";
  strptime(str, format, &tt);
  Date d = Date(tt.tm_mday, tt.tm_mon + 1, tt.tm_year + 1900, tt.tm_hour,
                tt.tm_min);
  return d;
}

bool DbNote::insertTable(Note & note)
{
  sqlite3_stmt *statement;
  string stmnt =
    "insert into NoteTable (date,NoteText,NoteTitle,NoteRef,KatKey) values (?,?,?,?,?)";

  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), stmnt.length(), &statement,
                               0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Insert-Statement failed");
    return false;
  }
  req = sqlite3_bind_text(statement, 1,
                          (note.getDate().getDateString()).c_str(),
                          (note.getDate().getDateString()).length(), SQLITE_TRANSIENT);
  if (req != SQLITE_OK) {
    throw SQLError("Binding Date into insert statement failed");
    return false;
  }
  req = sqlite3_bind_text(statement, 2, (note.getNoteText()).c_str(),
                          (note.getNoteText()).length(), SQLITE_TRANSIENT);
  if (req != SQLITE_OK) {
    throw SQLError("Binding note text into insert statement failed");
    return false;
  }
  req = sqlite3_bind_text(statement, 3, (note.getNotetitle()).c_str(),
                          (note.getNotetitle()).length(), SQLITE_TRANSIENT);
  if (req != SQLITE_OK) {
    throw SQLError("Binding note title into insert statement failed");
    return false;
  }
  req = sqlite3_bind_int(statement, 4, note.getNoteRef());
  if (req != SQLITE_OK) {
    throw SQLError("Binding note ref into insert statement failed");
    return false;
  }
  req = sqlite3_bind_int(statement, 5, note.getKatKey());
  if (req != SQLITE_OK) {
    throw SQLError(
      "Binding note category key into insert statement failed");
    return false;
  }
  req = sqlite3_step(statement);
  if (req != SQLITE_DONE) {
    throw SQLError("execute of insert statement failed");
    return false;
  }
  sqlite3_finalize(statement);
  sqlite3_close(db);
  return true;
}

bool DbNote::insertTable(Category & cat)
{
  sqlite3_stmt *statement;
  string stmnt = "insert into KatTable (KatDesc) values (?)";
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), stmnt.length(), &statement,
                               0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Insert-Statement failed");
    return false;
  }
  req = sqlite3_bind_text(statement, 1, (cat.getDesc()).c_str(),
                          (cat.getDesc()).length(), SQLITE_TRANSIENT);
  if (req != SQLITE_OK) {
    throw SQLError(
      "Binding category descripting into insert statement failed");
    return false;
  }
  req = sqlite3_step(statement);
  if (req != SQLITE_DONE) {
    throw SQLError("execute of insert statement failed");
    return false;
  }
  sqlite3_finalize(statement);
  
  stmnt = "select seq from sqlite_sequence where name = \"KatTable\"";
  req = sqlite3_prepare_v2(db, stmnt.c_str(), stmnt.length(), &statement,
                               0);
  if (req == SQLITE_ERROR) 
  {
    throw SQLError("Can't select data of the given table");
    return false;
  }
  sqlite3_column_count(statement);
  req = sqlite3_step(statement);
  if (req == SQLITE_ROW) 
  {
    int nextseq = (int) sqlite3_column_int(statement, 0);
    cat.setKatKey( nextseq );
  }
  else
  {
    throw SQLError("execute of select failed");
    return false;  
  }
  sqlite3_finalize(statement);
  sqlite3_close(db);
  return true;
}

bool DbNote::deleteRow(string sqlStmnt)
{
  sqlite3_stmt *statement;
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, sqlStmnt.c_str(), sqlStmnt.length(),
                               &statement, 0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Delete-Statement failed");
    return false;
  }
  req = sqlite3_step(statement);
  if (req != SQLITE_DONE) {
    throw SQLError("execute of delete statement failed");
    return false;
  }
  sqlite3_finalize(statement);
  sqlite3_close(db);
  return true;
}

bool DbNote::deleteNote(int idx)
{
  Note tmp_note;
  tmp_note.setNoteKey(idx);
  stringstream ss;
  ss << idx;
  string stmnt = "delete from NoteTable where NoteKey="+ss.str();
  if( existNote( tmp_note ) ) {
    return deleteRow(stmnt);
  }
  return false;
}

bool DbNote::updateRow(Note & note)
{
  sqlite3_stmt *statement;
  string stmnt =
    "update NoteTable SET date=?,NoteText=?,NoteTitle=?,NoteRef=?,KatKey=? where NoteKey=?;";
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), -1, &statement, 0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Update-Statement failed");
    return false;
  }
  req = sqlite3_bind_text(statement, 1,
                          (note.getDate().getDateString()).c_str(),
                          (note.getDate().getDateString()).length(), SQLITE_TRANSIENT);

  if (req != SQLITE_OK) {
    throw SQLError("Binding Date into update statement failed");
    return false;
  }
  req = sqlite3_bind_text(statement, 2, (note.getNoteText()).c_str(),
                          (note.getNoteText()).length(), SQLITE_TRANSIENT);
  if (req != SQLITE_OK) {
    throw SQLError("Binding note text into update statement failed");
    return false;
  }
  req = sqlite3_bind_text(statement, 3, (note.getNotetitle()).c_str(),
                          (note.getNotetitle()).length(), SQLITE_TRANSIENT);
  if (req != SQLITE_OK) {
    throw SQLError("Binding note title into update statement failed");
    return false;
  }
  req = sqlite3_bind_int(statement, 4, note.getNoteRef());
  if (req != SQLITE_OK) {
    throw SQLError("Binding note ref into update statement failed");
    return false;
  }
  req = sqlite3_bind_int(statement, 5, note.getKatKey());
  if (req != SQLITE_OK) {
    throw SQLError(
      "Binding note category key into update statement failed");
    return false;
  }
  req = sqlite3_bind_int(statement, 6, note.getNoteKey());
  if (req != SQLITE_OK) {
    throw SQLError("Binding note key into update statement failed");
    return false;
  }
  req = sqlite3_step(statement);
  if (req != SQLITE_DONE) {
    throw SQLError("execute of insert statement failed");
    return false;
  }
  sqlite3_finalize(statement);
  sqlite3_close(db);
  return true;
}

bool DbNote::updateRow(Category & cat)
{
  sqlite3_stmt *statement;
  string stmnt = "update KatTable set KatDesc=? where KatKey=?";
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), stmnt.length(), &statement,
                               0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Insert-Statement failed");
    return false;
  }
  req = sqlite3_bind_text(statement, 1, (cat.getDesc()).c_str(),
                          (cat.getDesc()).length(), SQLITE_TRANSIENT);
  if (req != SQLITE_OK) {
    throw SQLError(
      "Binding category descripting into update statement failed");
    return false;
  }
  req = sqlite3_bind_int(statement, 2, cat.getKatKey());
  if (req != SQLITE_OK) {
    throw SQLError("Binding kat key into update statement failed");
    return false;
  }
  req = sqlite3_step(statement);
  if (req != SQLITE_DONE) {
    throw SQLError("execute of insert statement failed");
    return false;
  }
  sqlite3_finalize(statement);
  sqlite3_close(db);
  return true;
}

bool DbNote::existNote(Note & note)
{
  sqlite3_stmt *statement;
  string stmnt = "select count(*) from NoteTable where NoteKey = ?";
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), -1, &statement, 0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing select-Statement failed");
    return false;
  }
  req = sqlite3_bind_int(statement, 1, note.getNoteKey());

  if (req != SQLITE_OK) {
    throw SQLError("Binding Date into select statement failed");
    return false;
  }
  req = sqlite3_step(statement);

  int noteCount = static_cast<int>(sqlite3_column_int(statement, 0));

  sqlite3_finalize(statement);
  sqlite3_close(db);
  if (noteCount > 0)
    return true;
  return false;
}

bool DbNote::existCategory(Category & category)
{
  sqlite3_stmt *statement;
  string stmnt = "select count(*) from KatTable where KatDesc = ?";
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), -1, &statement, 0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Select-Statement failed in existCategory()");
    return false;
  }
  
  req = sqlite3_bind_text(statement, 1, category.getDesc().c_str(), category.getDesc().size(), 0 );

  if (req != SQLITE_OK) {
    throw SQLError("Binding Date into select statement failed");
    return false;
  }
  req = sqlite3_step(statement);

  int catCount = static_cast<int>(sqlite3_column_int(statement, 0));

  sqlite3_finalize(statement);
  sqlite3_close(db);
  if (catCount > 0)
    return true;
  return false;


}

bool DbNote::saveCategory(Category & cat , save_t sav)
{
  if(sav == append_entr) {
    if( existCategory(cat)) {
      std::cout << "Category exists! Can't save this categeory !" << std::endl;
      return false;
    }
    if( ! insertTable( cat ) )
      return false;
    else
      return true;
  } else if( sav == write_entr ) {
    if( ! existCategory(cat) ) {
      std::cout << "Category doesn't exists! Can't override this categeory !" << std::endl;
      return false;
    }
    if( ! updateRow( cat ) ) {
      return false;
    } else
      return true;
  }
  return false;
}

bool DbNote::saveEntry(Note & note, save_t sav)
{
  Category cat;
  cat.setKatKey(note.getKatKey());
  
  if (! existCategory(cat)) {
    cout << "Category doesn't exists" << endl;
    return false;
  }
  if (note.getNoteRef() != 0) {
    Note ref_note;
    ref_note.setNoteKey(note.getNoteRef());
    if( ! existNote(ref_note) ) {
      std::cout << "Can't find given note reference" << std::endl;
      return false;
    }
  }
  if (sav == append_entr) {
    if (note.getNoteKey() != 0) {
      cout << "Note entry exists" << endl;
      return false;
    }
    if (!insertTable(note)) {
      cerr << "Insert notice failed" << endl;
      return false;
    }
  } else if (sav == write_entr) {
    if ( ! existNote(note) ) {
      cout << "Can't override notice cause of the notice doesn't exists"
           << endl;
      return false;
    }
    if (!updateRow( note)) {
      cerr << "Update the notice failed" << endl;
      return false;
    }
  }
  return true;
}

void DbNote::clearCategoryCache()
{
  if( categories.size() == 0 )
    return;
  categories.erase(categories.begin(),categories.begin() + categories.size());
}

void DbNote::clearNoteCache()
{
  if( notes.size() == 0 )
    return;
  notes.erase(notes.begin(),notes.begin() + notes.size());
}

string DbNote::getCategoryTitle(int key)
{
  clearCategoryCache();
  ostringstream s;
  s << "where KatKey =" << key;
  if ( ! selectCategories( s.str()) )
    return "";
  return categories[0].getDesc();
}

bool DbNote::createDB()
{
  sqlite3_stmt *statement;
  string stmnt = "CREATE TABLE KatTable(	KatKey INTEGER PRIMARY KEY\
										autoincrement,KatDesc TEXT);";
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), -1, &statement, 0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Select-Statement failed in existCategory()");
    return false;
  }
  req = sqlite3_step(statement);
  sqlite3_finalize(statement);

  stmnt = "CREATE TABLE NoteTable( NoteKey INTEGER PRIMARY KEY autoincrement,\
						date TEXT,\
						NoteText TEXT,\
						NoteTitle TEXT,\
						KatKey INTEGER,\
						NoteRef INTEGER,\
						FOREIGN KEY (KatKey) REFERENCES KatTable(KatKey));";
  req = sqlite3_prepare_v2(db, stmnt.c_str(), -1, &statement, 0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Select-Statement failed in existCategory()");
    return false;
  }
  req = sqlite3_step(statement);
  sqlite3_finalize(statement);
  sqlite3_close(db);
  Category c;
  c.setKatDesc("notice (default)");
  insertTable(c);
  return true;
}

int DbNote::getNextNoteKey( void )
{
  sqlite3_stmt *statement;
  string stmnt = "SELECT seq FROM SQLITE_SEQUENCE where name = \"NoteTable\"";
  if (!openDB()) {
    throw SQLError("Can't open the DB-Connection");
    return false;
  }
  int req = sqlite3_prepare_v2(db, stmnt.c_str(), -1, &statement, 0);
  if (req != SQLITE_OK) {
    throw SQLError("Preparing Select-Statement failed in getNextNoteKey()");
    return false;
  }
  req = sqlite3_step(statement);
  int noteKey = static_cast<int>(sqlite3_column_int(statement, 0));
  sqlite3_finalize(statement);
  sqlite3_close(db);
  return ( !noteKey ) ? 1 : noteKey;
}
