#include "myExport.h"

#include <iostream>
#include <string>
#include <vector>

extern std::vector<std::pair<std::string, std::string> > variableTable;

myExport::myExport() {
  std::vector<std::string> myArguments(0);
}  //class myExport constructor
myExport::myExport(std::vector<std::string> arg) {
  for (size_t i = 0; i < arg.size(); i++) {
    myArguments.push_back(arg[i]);
  }
}
void myExport::commandOperation() {
  if (checkExport()) {  //If there is no syntax error of export, export var
    exportVariable();
    //std::cout << "Export Success!" << std::endl;
  }
}

bool myExport::checkExport() {
  if (myArguments.size() != 2) {  //If argument number is not 2, print syntax error, return false
    std::cout << "Export syntax error! Format: inc var" << std::endl;
    return false;
  }
  for (size_t i = 0; i < myArguments[1].size();
       i++) {  //If var is invalid, print error, return false
    if (!((myArguments[1][i] >= '0' && myArguments[1][i] <= '9') ||
          (myArguments[1][i] >= 'a' && myArguments[1][i] <= 'z') ||
          (myArguments[1][i] >= 'A' && myArguments[1][i] <= 'Z') || myArguments[1][i] == '_')) {
      std::cout << "Export: invalid variable! Only letter, numbers and underscore" << std::endl;
      return false;
    }
  }
  return true;
}

void myExport::exportVariable() {
  std::vector<std::pair<std::string, std::string> >::iterator
      it;  //To find the var in variable table
  for (it = variableTable.begin(); it != variableTable.end(); it++) {
    if (it->first == myArguments[1]) {  //If found, export var to environment
      setenv(myArguments[1].c_str(), it->second.c_str(), 1);
      break;
    }
  }
  if (it == variableTable.end()) {
    std::cout << myArguments[1] << " is not found in variable table!"
              << std::endl;  //If not found, print error
    return;
  }
}
