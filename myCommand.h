#ifndef _MYCOMMAND_
#define _MYCOMMAND_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/* This class is the parent class of class myCd, mySet, myExport and myCd */
class myCommand
{
 protected:
  std::vector<std::string> myArguments;

 public:
  myCommand();
  myCommand(std::vector<std::string> arg);
  virtual void commandOperation(){};
  /* This function is realized in its child classes*/
  ~myCommand(){};
};

#endif
