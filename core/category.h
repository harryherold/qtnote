/*
 * File:   category.h
 * Author: harry
 *
 * Created on 6. April 2012, 10:37
 */

#ifndef CATEGORY_H
#define	CATEGORY_H
#include <string>
class Category
{
private:
  int KatKey;
  std::string KatDesc;
public:
  Category(std::string katdesc):KatDesc(katdesc) {}
  Category(int key):KatKey(key) {}
  Category() {}
  void setKatKey(int key);
  void setKatDesc(std::string desc);
  int getKatKey();
  std::string getDesc();
};


#endif	/* CATEGORY_H */

