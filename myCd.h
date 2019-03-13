#ifndef _MYCD_
#define _MYCD_

#include <unistd.h>

#include <iostream>
#include <vector>

#include "myCommand.h"

/* This class is to realize cd command */
class myCd : public myCommand
{
 public:
  myCd();
  myCd(std::vector<std::string> arg);
  virtual void commandOperation();
  /* This funtion takes parsed arguments
  * Check the syntax of cd
  * Change current directory
  */
  bool checkDirectory();
  /* This function check the syntax of cd
  * If the number of arguments is 2 return true, else return false
  */
  void changeDirectory();
  /* This function change directory with chdir()
  * Print error when failed
  */
  ~myCd();
};

#endif
