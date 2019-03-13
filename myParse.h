#ifndef _MYPARSE_
#define _MYPARSE_

#include <stdlib.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "runCommand.h"

/* This class is to parse commands get from prompt shell */
class myParse
{
 public:
  myParse(){};
  std::vector<std::string> splitPath(std::string & initpath, const std::string & simbol);
  /* This function is to split PATH by ':' */
  std::vector<std::string> replaceVariable(std::string & initCmd);
  /* This function is to replace $vars with their value in variable table */
  std::vector<std::string> parseArguments(std::string & initCmd);
  /* This function is to delete blanks and escape blanks with '\' */
  std::vector<runCommand> buildRunCommand(std::vector<std::string> & myarguments);
  /* This function seperate myArguments into a vector of runCommand by "|" 
  * runCommand is a class, one runCommand is a command to be executed
  * Every runCommand is parsed by parseRedirection()
  */
  runCommand parseRedirection(std::vector<std::string> & rcmd);
  /*This function parses redirection information of one runCommand */
  ~myParse(){};
};

#endif
