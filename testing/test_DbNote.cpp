#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE note
#include  <boost/test/unit_test.hpp>
#include "../core/DbNote.h"
#include "../core/note.h"
#include "../core/date.h"

BOOST_AUTO_TEST_CASE ( saveNoteEntry ) {
		DbNote *db = DbNote::getInstance();
		std::string s = "noticeDB";
		db->setDb(s);
		Note n;
		n.setNoteText("Heftig wichtig");
		n.setNoteTitle("Hans Huckebein");
		n.setNoteRef(0);
		n.setKatKey(2);
		Date * d = new Date();
		n.setDate( d );
		Category c;
		c.setKatDesc("BAR");
		db->selectNotes("where NoteKey = 100");
		std::vector<Note *> notes = db->getAllNotes();
		BOOST_CHECK (notes[0]->getNoteText() == "");
		BOOST_CHECK (notes[0]->getNotetitle() == "");
		BOOST_CHECK (notes[0]->getNoteRef() == 0);
		BOOST_CHECK (notes[0]->getNoteKey() == 100);
		Date *date = notes[0]->getDate();
		BOOST_CHECK (date->getMonth() == 0);
		BOOST_CHECK (date->getYear() == 0);
		BOOST_CHECK (date->getDay() == 0);
		BOOST_CHECK (date->getHour() == 0);
		BOOST_CHECK (date->getMinute() == 0);
		BOOST_CHECK( db->selectCategories("") == true );
		std::vector<Category *> cat = db->getAllCategories();
		std::cout << "size" << cat.size() << std::endl;
		BOOST_CHECK (cat[0]->getKatKey() == 1 );
		BOOST_CHECK (cat[0]->getDesc() == "notice" );
		std::cout << "desc " << cat[0]->getDesc() << std::endl;
		BOOST_CHECK (cat[1]->getKatKey() == 2 );
		BOOST_CHECK (cat[1]->getDesc() == "FFT" );
		BOOST_CHECK (cat[2]->getKatKey() == 3 );
		BOOST_CHECK (cat[2]->getDesc() == "Private" );
		
		BOOST_CHECK ( db->insertTable(&n) == true );
		BOOST_CHECK ( db->insertTable(&c) == true );
		BOOST_CHECK ( db->deleteRow("delete from KatTable where KatKey > 5") == true );
		BOOST_CHECK ( db->deleteRow("delete from NoteTable where NoteKey > 100") == true );
		
		n.setNoteText("Heftig wichtig");
		n.setNoteTitle("Hans Huckebein");
		n.setNoteRef(0);
		n.setKatKey(2);
		n.setNoteKey(1);
		BOOST_CHECK ( db->updateRow(&n) == true );
		Category cd;
		cd.setKatKey(4);
		cd.setKatDesc("viel spass");
		BOOST_CHECK ( db->updateRow(&cd) == true );
		/*BOOST_CHECK( db.saveEntry(n , c , append) == true );
		
		BOOST_CHECK( db.saveEntry(n , c , append) == false );
		
		BOOST_CHECK( db.saveEntry(n , c , write) == true );
		
		n.setNodeRef(2000);
		n.setNoteKey(2);
		BOOST_CHECK( db.saveEntry(n , c , write) == false );
		
		n.setNodeRef(1);
		BOOST_CHECK( db.saveEntry(n , c , append) == true );
		
		n.setNodeRef(-2000);
		BOOST_CHECK( db.saveEntry(n , c , write) == false );
		
		n.setNoteKey(-2);
		BOOST_CHECK( db.saveEntry(n , c , write) == false );
		
		c.setKatKey(-20);
		BOOST_CHECK( db.saveEntry(n , c , write) == false );	
		*/
}
