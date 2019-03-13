#ifndef _MYSHELL_
#define _MYSHELL_
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "myCd.h"
#include "myCommand.h"
#include "myExport.h"
#include "myInc.h"
#include "myParse.h"
#include "mySet.h"
#include "runCommand.h"

/*This class is to check command type and run command*/
class myShell : public myParse
{
 public:
  std::string InitCmd;
  std::string pathVal;
  std::vector<std::string> myArguments;
  std::vector<runCommand> allRunCommands;
  myCommand * mycmd;
  myShell();
  myShell(std::string initPath, std::string initCmd);
  myShell(myShell & cmd);
  void run();
  /* This function runs commands
  * First, get the parsed arguments
  * Then, check the syntax of redirection and pipe
  * Finally, check the type the command and run 
  */
  bool checkFilePipe();
  /* This function checks the syntax of redirection and pipe
  * If there is no syntax error, return true, else return false.
  */
  bool findCommand(int index, const char * pathVal);
  /* This function helps command without "./" or "/" to find its directory using PATH
    If the command is found, add its path before it and return true, else return false
  */
  bool checkRuncommand();
  /* This function is to check program begin with "./", "/" or nothing.
  * If begin without "./" and "/", find its directory using findCommand() function
  */
  void checkCommandType();
  /* This function is to check the command type and run
  * To check if it is cd, set, export, inc or program
  * If it is cd, set, export or inc, run command accordingly
  * If it is program, build a vector of command to run, 
  * after checking "./", "/" or nothing using checkRuncommand(), run all commands
  */
  void runAllCommand();
  /* This function is to fork, execve and wait
  */
  ~myShell(){};
};

#endif
