#ifndef _MYEXPORT_
#define _MYEXPORT_

#include <iostream>
#include <string>
#include <vector>

#include "myCommand.h"

/* This class is to realize export command */
class myExport : public myCommand
{
 public:
  myExport();
  myExport(std::vector<std::string> arg);
  virtual void commandOperation();
  /* This funtion takes parsed arguments
  * Check the syntax of export
  * Export var to environment
  */
  bool checkExport();
  /* This function check the syntax of export
  * If the number of arguments is 2 return true, else return false
  */
  void exportVariable();
  /* This function export var to environment with setenv()
  * If var is not found in variable table, print error
  */
  ~myExport(){};
};

#endif
