/*
 * File:   note.h
 * Author: harry
 *
 * Created on 6. April 2012, 10:44
 */

#ifndef NOTE_H
#define	NOTE_H
#include <string>
#include <vector>
#include <date.h>
class Note
{
private:
  int                       noteKey;
  Date                      date;
  std::string               noteText;
  std::string               noteTitle;
  int                       KatKey;
  int                       noteRef;

public:
  Note();
  Note( Date d );
  void                      setNoteKey(int key);
  void                      setKatKey(int katkey);
  void                      setDate (Date d);
  void                      setNoteText(std::string text);
  void                      setNoteTitle(std::string title);
  void                      setNoteRef(int ref);
  
  int                       getNoteRef();
  Date&                     getDate();
  int                       getKatKey();
  int                       getNoteKey();
  std::string               getNotetitle();
  std::string               getNoteText();
  std::vector<std::string>  formatText();
};


#endif	/* NOTE_H */

