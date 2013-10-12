#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE note
#include  <boost/test/unit_test.hpp>
#include "../core/date.h"


BOOST_AUTO_TEST_CASE ( testdate ) {
		Date *d = new Date();
		time_t now = time(NULL);
		tm *tt = localtime(&now);
		BOOST_REQUIRE(tt->tm_min == d->getMinute());
		BOOST_REQUIRE(tt->tm_hour == d->getHour());
		BOOST_REQUIRE( (tt->tm_year + 1900) == d->getYear());
		BOOST_REQUIRE( (tt->tm_mon + 1) == d->getMonth());
		BOOST_REQUIRE( tt->tm_mday == d->getDay() );
		delete d;
}
