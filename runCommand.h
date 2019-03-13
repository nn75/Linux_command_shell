#ifndef _RUNCOMMAND_
#define _RUNCOMMAND_

#include <stdlib.h>

#include <cstring>
#include <string>
#include <vector>

/* This class includes the information of one command to run */
class runCommand
{
 public:
  std::string inputfile;
  std::string outputfile;
  std::string error_outputfile;
  std::vector<std::string>
      strArgv;  //strArgv will be transferred from vector<string >to char** to be passed into execve
  runCommand();
  runCommand(std::string & input,
             std::string & output,
             std::string & error_output,
             std::vector<std::string> & sArgv);
  runCommand(const runCommand & rcmd);
  ~runCommand(){};
};

#endif
