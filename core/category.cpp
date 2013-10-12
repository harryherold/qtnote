#include "category.h"
void Category::setKatKey(int key)
{
  KatKey = key;
}
void Category::setKatDesc(std::string desc)
{
  KatDesc = desc;
}
int Category::getKatKey()
{
  return KatKey;
}
std::string Category::getDesc()
{
  return KatDesc;
}
