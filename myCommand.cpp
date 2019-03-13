#include "myCommand.h"

myCommand::myCommand() {
  std::vector<std::string> myArguments(0);
}
myCommand::myCommand(std::vector<std::string> arg) {
  for (size_t i = 0; i < arg.size(); i++) {
    myArguments[i] = arg[i];
  }
}
