/***************************************************************

                      * ECE551 mp_miniproject
                      * NetID: nn75
                      * Date: 12/3/2018
      
****************************************************************/
#include "myShell.h"

extern char ** environ;
std::vector<std::pair<std::string, std::string> >
    variableTable;  //The vector of pairs to hold variable set

int main(int argc, char * argv[]) {
  char pwd[255];
  if (!getcwd(pwd, 255)) {  //Get absolute path
    perror("getcwd");
    return 1;
  }
  std::cout << "myShell: " << pwd << " $ ";  //Print prompt
  std::string pathVal = getenv("PATH");
  std::string initCmd;
  while (getline(std::cin, initCmd) &&
         initCmd != "exit") {  //Get initial command from shell with exit conditions
    myShell cmd(pathVal, initCmd);
    if (cmd.InitCmd == "") {
      std::cout << "myShell: " << pwd << " $ ";  //If empty, print line and continue
      continue;
    }
    cmd.run();  //Run command
    if (!getcwd(pwd, 255)) {
      perror("getcwd");
    }
    std::cout << "myShell: " << pwd << " $ ";
  }
  if (initCmd != "exit") {
    std::cout << std::endl;
  }
  return EXIT_SUCCESS;
}
