#include "myParse.h"

extern std::vector<std::pair<std::string, std::string> > variableTable;

/* This function is to split PATH by ':' */
std::vector<std::string> myParse::splitPath(std::string & initpath, const std::string & simbol) {
  std::vector<std::string> splitedPath;  //split initpath and store every path into splitedPath
  std::string str = initpath;
  if ("" == str) {
    return splitedPath;
  }
  char * strcopy = new char[str.length() + 1];
  strcpy(strcopy, str.c_str());

  char * simcopy = new char[simbol.length() + 1];
  strcpy(simcopy, simbol.c_str());

  char * p = strtok(strcopy, simcopy);  //split initpath into paths by simbol
  while (p != NULL) {
    std::string s = p;
    splitedPath.push_back(s);
    p = strtok(NULL, simcopy);
  }
  delete[] strcopy;
  delete[] simcopy;
  delete[] p;
  return splitedPath;
}

/*Replace the "$" marked variables with environment variables.*/
std::vector<std::string> myParse::replaceVariable(std::string & initCmd) {
  size_t i = 0;
  size_t j = 0;
  std::string str_formal = "";
  std::string str_temp = initCmd;  //str_temp is a string of replaced argument for further parse
  std::string str_envname = "";    //str_envname is a string to store var's name
  std::string str_replace = "";    //str_replace is a string to replace $var
  std::vector<std::string> str_notfound;
  bool dollarSignFound = true;  //a flag goes false if there is no $var to replace
  bool varFound = false;   //a flag goes true when $var is found in the variable table
  while (dollarSignFound) {
    str_formal = "";
    dollarSignFound = false;
    for (i = 0; i < str_temp.size(); i++) {
      varFound = false;
      if (str_temp[i] == '\\') {  //reserve "\" in argument
        i++;
        str_formal = str_formal + "\\" + str_temp[i];
      }
      else if (str_temp[i] == '$') {
        dollarSignFound = true;
        j = 0;
        str_envname = "";
        str_replace = "";
        while (((str_temp[i + 1 + j] >= '0' && str_temp[i + 1 + j] <= '9') ||
                (str_temp[i + 1 + j] >= 'a' && str_temp[i + 1 + j] <= 'z') ||
                (str_temp[i + 1 + j] >= 'A' && str_temp[i + 1 + j] <= 'Z') ||
                str_temp[i + 1 + j] == '_') &&
               (i + 1 + j) <
                   str_temp.size()) {  //If "$" is found, find the longest valid name behind it
          str_envname = str_envname + str_temp[i + 1 + j];
          j++;
        }
        if (str_envname == "") {  //If there is no valid name behind "$", add "$" back to command
          str_formal += "$";
          if (str_temp[i + 1] != '$') {  //If there is no "$" behind that "$", set dollarSignFound false
            dollarSignFound = false;
          }
          continue;
        }
        if (getenv(str_envname.c_str()) !=
            NULL) {  //If var is found in environment, str_replace equals value, fing_flag is true
          str_replace = getenv(str_envname.c_str());
          varFound = true;
        }
        else {
          str_replace = "";  //If var is not found, str_replace sets ""
        }
        std::vector<std::pair<std::string, std::string> >::iterator it;
        for (
            it = variableTable.begin(); it != variableTable.end();
            it++) {  //If var is found in current table, str_replace equals value, fing_flag is true
          if (it->first == str_envname) {
            str_replace = it->second;
            varFound = true;
            break;
          }
        }
        if (!varFound) {  //If $var is not found first time, report error
          dollarSignFound = false;
          std::vector<std::string>::iterator it1;
          for (it1 = str_notfound.begin(); it1 != str_notfound.end(); it1++) {
            if (*it1 == str_envname) {
              break;
            }
          }
          if (it1 == str_notfound.end()) {
            std::cout << str_envname << " not found" << std::endl;
            str_notfound.push_back(str_envname);
          }
        }
        str_formal = str_formal + str_replace;  //replace $var with its value
        i = i + j;
      }
      else {
        str_formal = str_formal + str_temp[i];  //If it is not "$" and "/", add back to the command
      }
    }
    str_temp = str_formal;
  }
  return parseArguments(str_temp);  //return "$" replaced command for further parse
}

/* Seperate arguments by ' '(considering '\') */
std::vector<std::string> myParse::parseArguments(std::string & initCmd) {
  std::vector<std::string> parsedArguments;  //Stores parsed arguments
  size_t i = 0;
  std::string str = "";  //Stores one argument
  while (i < initCmd.size()) {
    if (initCmd[i] == ' ') {  //Meet space, push back the argument to the vector
      if (str == "") {
        i++;
        continue;
      }
      parsedArguments.push_back(str);
      str = "";
    }
    else if (initCmd[i] == '\\') {  //Meet slash, reserve anything behind it
      i++;
      str = str + initCmd[i];
    }
    else {
      str = str + initCmd[i];  //Else, add letters to argument
    }
    i++;
  }
  if (str != "") {
    parsedArguments.push_back(str);
  }
  if (parsedArguments.size() == 0) {
    parsedArguments.push_back("");
  }
  return parsedArguments;
}

/* Seperate myArguments by "|" and build a vector of commands to run*/
std::vector<runCommand> myParse::buildRunCommand(std::vector<std::string> & myarguments) {
  std::vector<runCommand> splitedRunCommand;  //A vector of runCommand
  std::vector<std::string> temp;              //Stores arguments temporarily for one runCommand
  for (size_t i = 0; i < myarguments.size(); i++) {
    if (myarguments[i] == "|") {  //Meet "|", build a new runCommand
      runCommand rc = parseRedirection(temp);
      splitedRunCommand.push_back(rc);
      temp.clear();
    }
    else {
      temp.push_back(myarguments[i]);  //Else, push arguments for one runCommand
    }
  }
  runCommand rc(parseRedirection(temp));
  splitedRunCommand.push_back(rc);
  return splitedRunCommand;
}

/*Parse redirection in every runCommand*/
runCommand myParse::parseRedirection(std::vector<std::string> & rcmd) {
  size_t i = 0;
  std::string inputfile = "";         //Stores name of input file
  std::string outputfile = "";        //Stores name of output file
  std::string error_outputfile = "";  //Stores name of error output file
  std::vector<std::string> str;
  for (i = 0; i < rcmd.size(); i++) {
    if (rcmd[i] == "<") {  //Meet "<", next argument is assigned to inputfile name
      i++;
      inputfile = rcmd[i];
    }
    else if (rcmd[i] == ">") {  //Meet ">", next argument is assigned to outputfile name
      i++;
      outputfile = rcmd[i];
    }
    else if (rcmd[i] == "2>") {  //Meet "2>", next argument is assigned to error outputfile name
      i++;
      error_outputfile = rcmd[i];
    }
    else {
      str.push_back(rcmd[i]);  //Else push argument back
    }
  }
  runCommand oneRunCommand(
      inputfile, outputfile, error_outputfile, str);  //Build a runCommand, initialize it
  return oneRunCommand;
}
