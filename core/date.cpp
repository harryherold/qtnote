#include "date.h"
#include <time.h>
#include <sstream>
#include <stdio.h>
Date::Date(int d, int m, int y, int h, int min) {
	day = d;
	month = m;
	year = y;
	this->min = min;
	hour = h;
}
Date::Date() {
	time_t now = time(NULL);
	tm *tt = localtime(&now);
	year = tt->tm_year + 1900;
	month = tt->tm_mon + 1;
	day = tt->tm_mday;
	min = tt->tm_min;
	hour = tt->tm_hour;
}
Date::Date(const Date& d) {
	day = d.day;
	month = d.month;
	year = d.year;
	hour = d.hour;
	min = d.min;
}
Date & Date::operator =(const Date & d) {
	day = d.day;
	month = d.month;
	year = d.year;
	hour = d.hour;
	min = d.min;
	return *this;
}
void Date::setDate(int d, int m, int y) {
	day = d;
	month = m;
	year = y;
}
void Date::setTime(int h, int min) {
	this->min = min;
	hour = h;
}
int Date::getDay() {
	return day;
}
int Date::getMonth() {
	return month;
}
int Date::getYear() {
	return year;
}
int Date::getHour() {
	return hour;
}
int Date::getMinute() {
	return min;
}
std::string Date::getDateString() {
	std::ostringstream date_text;
	date_text << year << "-";
	if (month < 10)
		date_text << "0";
	date_text << month << "-";
	if (day < 10)
		date_text << "0";
	date_text << day << " ";
	if (hour < 10)
		date_text << "0";
	date_text << hour << ":";
	if (min < 10)
		date_text << "0";
	date_text << min;

	return date_text.str();
}
