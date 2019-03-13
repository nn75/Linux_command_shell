#include "runCommand.h"

runCommand::runCommand() {
  inputfile = "";
  outputfile = "";
  error_outputfile = "";
  std::vector<std::string> strArgv(0);
}
runCommand::runCommand(std::string & input,
                       std::string & output,
                       std::string & error_output,
                       std::vector<std::string> & sArgv) {
  inputfile = input;
  outputfile = output;
  error_outputfile = error_output;
  for (size_t i = 0; i < sArgv.size(); i++) {
    strArgv.push_back(sArgv[i]);
  }
}
runCommand::runCommand(const runCommand & rcmd) {
  inputfile = rcmd.inputfile;
  outputfile = rcmd.outputfile;
  error_outputfile = rcmd.error_outputfile;
  for (size_t i = 0; i < rcmd.strArgv.size(); i++) {
    strArgv.push_back(rcmd.strArgv[i]);
  }
}
