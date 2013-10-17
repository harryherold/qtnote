/*
 * File:   DbNote.h
 * Author: Christian Herold
 *
 * Created on 1. April 2012, 16:07
 * 
 * Use Bitmapping to decide between todo , note and event
 */

#ifndef DBNOTE_H
#define	DBNOTE_H

#include <sqlite3.h>
#include <sstream>

#include <string>
#include <vector>

#include <util.h>
#include <note.h>
#include <category.h>

using namespace std;

class DbNote
{

public:
  static DbNote*    getInstance();
  Date              extractDate(char * str);
  bool              dbExists();
  void              setDb(string fileName);

  bool              saveEntry(Note & note, save_t sav);
  bool              saveCategory(Category & cat, save_t sav);
  int               getNextNoteKey( void );
  
  vector<Note>      getEntriesByCategory(Category cat);
  vector<Note>      getAllEntriesOverview(void);
  vector<Note>      searchEntry(string title, string text);

  string            getCategoryTitle(int key);
  void              clearCategoryCache();
  void              clearNoteCache();
  bool              insertTable(Category & cat);
  bool              insertTable(Note & note);
  bool              selectNotes(string where);
  bool              existNote(Note & note);
  bool              existCategory(Category & category);
  bool              selectCategories(string where);
  bool              deleteRow(string sqlStmnt);
  bool              updateRow(Note & note);
  bool              updateRow(Category & cat);
  bool              deleteNote(int idx);
  bool              deleteCategory(int idx);
  bool              openDB(void);
  bool              createDB(void);
  void              closeDB(void);

  vector<Note>&     getAllNotes(void);
  vector<Category>& getAllCategories();
  
private:
  sqlite3*          db;
  vector<Note>      notes;
  vector<Category>  categories;
  static DbNote*    singleton;
  string            file;
  
  DbNote();
  ~DbNote();
};
#endif	/* DBNOTE_H */

