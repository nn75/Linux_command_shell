#ifndef _MYSET_
#define _MYSET_
#include <iostream>
#include <string>
#include <vector>

#include "myCommand.h"

/* This class is to realize set command */
class mySet : public myCommand
{
 public:
  mySet();
  mySet(std::vector<std::string> arg);
  virtual void commandOperation();
  /* This funtion takes parsed arguments
  * Check the syntax of set
  * Add '\' back to the front of ' ' of value
  * Then set var value
  */
  bool checkSet();
  /* This function check the syntax of set
  * If the number of arguments more than 2 return true, else return false
  * If the name of var is valid return true, else print error and return false
  */
  void setVariable(std::pair<std::string, std::string> currPair);
  /* This function realize set
  * If var is already in variable table, update its value
  * If var is not currently in variable table, add var and its value to table
  */
  ~mySet(){};
};

#endif
