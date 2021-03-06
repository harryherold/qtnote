/*
 * note.cpp
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
#include  <note.h>

Note::Note( Date d )
    :noteKey(0),date(d),KatKey(0),noteRef(0)
{ }
Note::Note()
    :noteKey(0),KatKey(0),noteRef(0)
{ }

void Note::setNoteKey( int key )
{
    noteKey = key;
}
void Note::setKatKey( int katkey )
{
    KatKey=katkey;
}
void Note::setNoteText(std::string text)
{
    if( text.length() != 0)
        noteText = text;
}
void Note::setNoteTitle(std::string title)
{
    if( title.length() != 0 )
        noteTitle = title;
}
void Note::setNoteRef(int ref)
{
    noteRef = ref;
}
void Note::setDate (Date d)
{
    date = d;
}
Date & Note::getDate ()
{
    return date;
}
int Note::getNoteRef()
{
    return noteRef;
}
int Note::getNoteKey()
{
    return noteKey;
}
std::string Note::getNotetitle()
{
    return noteTitle;
}
std::string Note::getNoteText()
{
    return noteText;
}
int Note::getKatKey()
{
    return KatKey;
}
std::vector<std::string> Note::formatText()
{
    size_t start_pos = 0;
    size_t last_pos = 10;
    std::vector<std::string> notes;
    std::string str = noteText;
    if(str.length() > 80)
    {
        while ((str.length() - last_pos) > 80)
        {
            int min = (last_pos == 10) ? 10 : 0;
            size_t max = (last_pos - min) + 80;
            for (unsigned int i = last_pos; i < max; i++)
            {
                size_t tmp_pos = str.find(' ', i);
                if (tmp_pos != std::string::npos)
                {
                    if (tmp_pos < max)
                        last_pos = tmp_pos;
                    i = tmp_pos;
                }
            }
            notes.push_back(str.substr(start_pos,last_pos - start_pos));
            start_pos = last_pos + 1 ;
            last_pos++;
        }
        if(last_pos < str.length())
            notes.push_back(str.substr(start_pos,str.length() - start_pos));

        return notes;
    }
    else
    {
        notes.push_back(str);
        return notes;
    }
}
