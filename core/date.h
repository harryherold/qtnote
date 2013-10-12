#ifndef DATE_H
#define	DATE_H
#include <string>
class Date {

private:
	int month;
	int day;
	int year;
	int min;
	int hour;
public:
	Date();
	Date(const Date& d);
	Date& operator = (const Date & d);
	Date(int d , int m , int y , int h , int min);
	void setDate(int d, int m , int y);
	void setTime(int h , int min);
	int getYear();
	int getMonth();
	int getDay();
	int getHour();
	int getMinute();
	std::string getDateString();
};


#endif	/* DATE_H */
