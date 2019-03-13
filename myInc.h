#ifndef _MYINC_
#define _MYINC_

#include <iostream>
#include <sstream>
#include <vector>

#include "myCommand.h"

/* This class is to realize inc command */
class myInc : public myCommand
{
 public:
  myInc();
  myInc(std::vector<std::string> arg);
  virtual void commandOperation();
  /* This funtion takes parsed arguments
  * Check the syntax of inc
  * Then increment var's value
  */
  bool checkVal();
  /* This function check the syntax of inc
  * If the number of arguments is 2 return true, else return false
  * If the name of var is valid return true, else print error and return false
  */
  void incVariable();
  /* This function realize increment
  * If var is currently in variable table and the value of var as a number in base10, increment it
  * If var is not currently in variable table, or does not represent a number in base10, then its intiial value is 0 (incremented to 1)
  */
  ~myInc(){};
};

#endif
