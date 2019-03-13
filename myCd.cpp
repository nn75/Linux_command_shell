#include "myCd.h"

#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

myCd::myCd() {
  std::vector<std::string> myArguments(0);
}
myCd::myCd(std::vector<std::string> arg) {
  for (size_t i = 0; i < arg.size(); i++) {
    myArguments.push_back(arg[i]);
  }
}

void myCd::commandOperation() {
  if (checkDirectory()) {  //If there is no syntax error of cd, change current directory
    changeDirectory();
  }
}

bool myCd::checkDirectory() {
  if (myArguments.size() != 2) {  //If argument number is not 2, print syntax error, return false
    std::cout << "Cd syntax error! Format: cd directory" << std::endl;
    return false;
  }
  return true;  //If argument number is 2, return true
}

void myCd::changeDirectory() {
  int change_success;
  change_success = chdir(myArguments[1].c_str());  //Change current directory
  if (change_success) {                            //If cd failed, print cause of error
    perror("cd");
  }
}

myCd::~myCd() {}
