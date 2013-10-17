/*
 * DbNote.h
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

