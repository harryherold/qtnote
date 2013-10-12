#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE note
#include  <boost/test/unit_test.hpp>
#include "../core/note.h"
#include "../core/date.h"
BOOST_AUTO_TEST_CASE ( test1 ) {
		Note  *n1 = new Note();
		//n1->setNoteKey(10);
		//BOOST_CHECK ( n1->getNoteKey() == 10 );
		Date *d = new Date();
		n1->setDate(*d);
		Date d2 = n1->getDate();
		BOOST_REQUIRE( d2.getMinute() == d->getMinute() );
		delete n1, d;
} 
