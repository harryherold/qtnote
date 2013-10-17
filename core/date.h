/*
 * date.h
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
#ifndef DATE_H
#define	DATE_H
#include <string>
class Date
{

private:
    int         month;
    int         day;
    int         year;
    int         min;
    int         hour;
public:
    Date();
    Date(const Date& d);

    Date(int d , int m , int y , int h , int min);

    Date&       operator = (const Date & d);

    void        setDate(int d, int m , int y);
    void        setTime(int h , int min);
    int         getYear();
    int         getMonth();
    int         getDay();
    int         getHour();
    int         getMinute();
    std::string getDateString();
};


#endif	/* DATE_H */
