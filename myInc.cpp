#include "myInc.h"

extern std::vector<std::pair<std::string, std::string> > variableTable;

myInc::myInc() {
  std::vector<std::string> myArguments(0);
}  //class myInc constructor
myInc::myInc(std::vector<std::string> arg) {
  for (size_t i = 0; i < arg.size(); i++) {
    myArguments.push_back(arg[i]);
  }
}

void myInc::commandOperation() {  //If there is no syntax error and name error of inc, inc var
  if (checkVal()) {
    incVariable();
    //std::cout << "Increase variable success!" << std::endl;
  }
}

bool myInc::checkVal() {
  if (myArguments.size() != 2) {  //If argument number is not 2, print syntax error, return false
    std::cout << "Inc syntax error! Format: inc var" << std::endl;
    return false;
  }
  for (size_t i = 0; i < myArguments[1].size();
       i++) {  //If var is invalid, print error, return false
    if (!((myArguments[1][i] >= '0' && myArguments[1][i] <= '9') ||
          (myArguments[1][i] >= 'a' && myArguments[1][i] <= 'z') ||
          (myArguments[1][i] >= 'A' && myArguments[1][i] <= 'Z') || myArguments[1][i] == '_')) {
      std::cout << "Inc: invalid variable! Only letter, numbers and underscore" << std::endl;
      return false;
    }
  }
  return true;
}

void myInc::incVariable() {
  std::vector<std::pair<std::string, std::string> >::iterator it;
  std::stringstream ss;
  for (it = variableTable.begin(); it != variableTable.end(); it++) {
    if (it->first == myArguments[1]) {  //If var is in current variable table
      for (size_t i = 0; i < it->second.size(); i++) {
        if (it->second[i] < '0' &&
            it->second[i] > '9') {  //If value is not number in base10, increment it to 1.
          it->second = "1";
          return;
        }
      }
      ss << (atoi(it->second.c_str()) + 1);  //If value is number in base10, increment one.
      it->second = ss.str();
    }
  }
  if (it ==
      variableTable
          .end()) {  //If var is not in current variable table, set its value 1, add it to table
    std::pair<std::string, std::string> incPair(myArguments[1], "1");
    variableTable.push_back(incPair);
  }
}
