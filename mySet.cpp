#include "mySet.h"

#include <iostream>
#include <string>
#include <vector>

extern std::vector<std::pair<std::string, std::string> > variableTable;

mySet::mySet() {
  std::vector<std::string> myArguments(0);
}  //class mySet constructor
mySet::mySet(std::vector<std::string> arg) {
  for (size_t i = 0; i < arg.size(); i++) {
    myArguments.push_back(arg[i]);
  }
}

void mySet::commandOperation() {
  if (checkSet()) {  //If set var without value, set var ""
    if (myArguments.size() == 2) {
      std::pair<std::string, std::string> setPair(myArguments[1], "");
      setVariable(setPair);
    }
    else {
      std::string combineVar = "";
      std::string str = "";
      size_t j = 2;
      for (j = 2; j < myArguments.size() - 1; j++) {
        str = "";  //Connect values after var together
        for (size_t i = 0; i < myArguments[j].size(); i++) {
          if (myArguments[j][i] == ' ') {  //If there is blank in value, add '\' back before blank
            str = str + '\\';              //Because blanks in value has been delete '\',
          }  //To realize blanks displayed(by echo var, etc.), add '\' back before blank
          str = str + myArguments[j][i];
        }
        combineVar = combineVar + str + " ";
      }
      str = "";
      for (size_t i = 0; i < myArguments[j].size(); i++) {
        if (myArguments[j][i] == ' ') {
          str = str + '\\';
        }
        str = str + myArguments[j][i];
      }
      combineVar = combineVar + str;
      std::pair<std::string, std::string> setPair(
          myArguments[1], combineVar);  //Add val and its parsed value to variable table
      setVariable(setPair);
    }
    //std::cout << "Set variable success!" << std::endl;
  }
}

bool mySet::checkSet() {
  if (myArguments.size() <
      2) {  //If argument number is less than 2, print syntax error, return false
    std::cout << "Set Syntax error! Format: set var value" << std::endl;
    return false;
  }
  for (size_t i = 0; i < myArguments[1].size();
       i++) {  //If var is invalid, print error, return false
    if (!((myArguments[1][i] >= '0' && myArguments[1][i] <= '9') ||
          (myArguments[1][i] >= 'a' && myArguments[1][i] <= 'z') ||
          (myArguments[1][i] >= 'A' && myArguments[1][i] <= 'Z') || myArguments[1][i] == '_')) {
      std::cout << "Invalid variable name! Only letters, numbers and underscore" << std::endl;
      return false;
    }
  }
  return true;
}

void mySet::setVariable(std::pair<std::string, std::string> currPair) {
  std::vector<std::pair<std::string, std::string> >::iterator it;
  for (it = variableTable.begin(); it != variableTable.end(); it++) {
    if (it->first == currPair.first) {  //If var is already in variable table, update its value
      it->second = currPair.second;
      break;
    }
  }
  if (it ==
      variableTable
          .end()) {  //If var is not currently in variable table, add var and its value to table
    variableTable.push_back(currPair);
  }
}
