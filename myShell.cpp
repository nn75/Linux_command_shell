#include "myShell.h"

extern char ** environ;

myShell::myShell() {  //constructor of class myShell
  InitCmd = "";
  pathVal = "";
  mycmd = NULL;
  std::vector<std::string> myArguments(0);
  std::vector<runCommand> allRunCommands(0);
}
myShell::myShell(std::string initPath, std::string initCmd) {
  InitCmd = initCmd;
  pathVal = initPath;
  mycmd = NULL;
  std::vector<std::string> myArguments(0);
  std::vector<runCommand> allRunCommands(0);
}
myShell::myShell(myShell & cmd) {  //copy constructor of class myShell
  InitCmd = cmd.InitCmd;
  pathVal = cmd.pathVal;
  mycmd = cmd.mycmd;
  myArguments = cmd.myArguments;
  allRunCommands = cmd.allRunCommands;
}

/* This function runs commands*/
void myShell::run() {
  myArguments = replaceVariable(InitCmd);  //Get the parsed arguments
  if (checkFilePipe()) {                   //Check the syntax error of redirection and pipe
    checkCommandType();                    //Check the type of command and run
  }
}

/* This function checks the syntax of redirection and pipe*/
bool myShell::checkFilePipe() {
  for (size_t i = 0; i < myArguments.size(); i++) {
    if (myArguments[i] == "<" || myArguments[i] == ">" ||
        myArguments[i] == "2>") {  //If there is "<", ">" and "2>" in arguments
      if (i == 0 ||
          i ==
              myArguments.size() -
                  1) {  //check if it is the first or the last argument, if it is report syntax error
        std::cout << "Redirection syntex error! Format: < ( or > or 2> ) filename" << std::endl;
        return false;
      }
      else {  //If in the middle, check its front and back argument, if there is  "<", ">" , "2>" or "|", report syntax error
        if (myArguments[i - 1] == "<" || myArguments[i - 1] == ">" || myArguments[i - 1] == "2>" ||
            myArguments[i - 1] == "|" || myArguments[i + 1] == "<" || myArguments[i + 1] == ">" ||
            myArguments[i + 1] == "2>" || myArguments[i + 1] == "|") {
          std::cout << "Redirection syntex error! Format: < ( or > or 2> ) filename" << std::endl;
          return false;
        }
      }
    }
    if (myArguments[i] == "|") {  //If there is "|" in arguments
      if (i == 0 ||
          i ==
              myArguments.size() -
                  1) {  //check if it is the first or the last argument, if it is report syntax error
        std::cout << "Pipe syntex error! Format: command | command" << std::endl;
        return false;
      }
      else {  //If in the middle, check its front and back argument, if there is  "<", ">" , "2>" or "|", report syntax error
        if (myArguments[i - 1] == "<" || myArguments[i - 1] == ">" || myArguments[i - 1] == "2>" ||
            myArguments[i - 1] == "|" || myArguments[i + 1] == "<" || myArguments[i + 1] == ">" ||
            myArguments[i + 1] == "2>" || myArguments[i + 1] == "|") {
          std::cout << "Pipe syntex error! Format: command | command" << std::endl;
          return false;
        }
      }
    }
  }
  return true;
}

/* This funtion helps find program in PATH */
bool myShell::findCommand(int index, const char * pathVal) {
  DIR * curr;
  struct dirent * ent;
  if ((curr = opendir(pathVal)) == 0) {  //If open directory failed, report error
    std::cout << "Open directory failed!" << std::endl;
    closedir(curr);
    exit(EXIT_FAILURE);
  }
  while ((ent = readdir(curr)) != NULL) {
    std::string nextDir(pathVal);
    std::string fileName(ent->d_name);
    if (fileName.compare(".") == 0 || fileName.compare("..") == 0) {
      continue;
    }
    bool isfound = false;
    switch (ent->d_type) {
      case DT_DIR:  //If fileName is a directory, add path before it and open it
        nextDir = nextDir + "/" + fileName;
        isfound = findCommand(
            index, nextDir.c_str());  //Recurse until program is found or all file searched
        if (isfound) {
          closedir(curr);
          return true;
        }
        break;
      case DT_LNK:
        break;
      default:  //If fileName is not a directory and link
        if (fileName == allRunCommands[index]
                            .strArgv[0]) {  //If found, add path before the program and return true
          allRunCommands[index].strArgv[0] = nextDir + "/" + allRunCommands[index].strArgv[0];
          closedir(curr);
          return true;
        }
        break;
    }
  }
  closedir(curr);  //If not found in all path, return false
  return false;
}

/* This function is to check program begin with "./", "/" or nothing.*/
bool myShell::checkRuncommand() {
  std::vector<runCommand>::iterator it;
  std::vector<std::string> pathVec = splitPath(pathVal, ":");
  int index = 0;
  for (it = allRunCommands.begin(); it != allRunCommands.end(); it++) {
    if (it->strArgv[0][0] == '.') {
      if (it->strArgv[0][1] == '/') {  //If program begin with "./", continue
        index++;
        continue;
      }
      else {  //If program begin with "." but not followed by "/", report error
        std::cout << "Command " << it->strArgv[0] << " not found" << std::endl;
        break;
      }
    }
    else if (it->strArgv[0][0] == '/') {  //If program begin with "/", continue
      index++;
      continue;
    }
    else {
      size_t i = 0;
      for (i = 0; i < pathVec.size();
           ++i) {  //If program is not begin with "./" or "/", find program in PATH
        if (findCommand(index, pathVec[i].c_str())) {
          break;
        }
      }
      if (i == pathVec.size()) {  //If program is not found in PATH, return false
        std::cout << "Command " << it->strArgv[0] << " not found" << std::endl;
        break;
      }
    }
    index++;
  }
  if (it != allRunCommands.end()) {
    return false;
  }
  return true;
}

/* This function is to check the command type and run */
void myShell::checkCommandType() {  //Type cd
  if (myArguments[0] == "cd") {
    myCd mycd(myArguments);
    mycmd = &mycd;
    mycmd->commandOperation();
  }
  else if (myArguments[0] == "set") {  //Type set
    mySet myset(myArguments);
    mycmd = &myset;
    mycmd->commandOperation();
  }
  else if (myArguments[0] == "export") {  //Type export
    myExport myexport(myArguments);
    mycmd = &myexport;
    mycmd->commandOperation();
  }
  else if (myArguments[0] == "inc") {  //Type inc
    myInc myinc(myArguments);
    mycmd = &myinc;
    mycmd->commandOperation();
  }
  else if (myArguments[0] == "") {
    return;
  }
  else {  //Type "./", "/" or program name
    allRunCommands = buildRunCommand(myArguments);
    if (checkRuncommand()) {
      runAllCommand();
    }
  }
}

/* This function is to fork, execve and wait */
void myShell::runAllCommand() {
  int status = 0;
  int input_fd = 0;
  int output_fd = 0;
  int error_fd = 0;
  size_t pipeSize = 0;
  pipeSize = allRunCommands.size();
  int * pipe_fds = new int[2 * (pipeSize - 1)];  //an array of pipefds
  pid_t * pids = new pid_t[pipeSize];            //an array of pids
  for (size_t i = 0; i < pipeSize - 1; i++) {
    if (pipe(pipe_fds + i * 2) == -1) {
      perror("I couldn't pipe! ");
      exit(EXIT_FAILURE);
    }
  }
  for (size_t k = 0; k < pipeSize; k++) {
    pids[k] = fork();  //fork
    if (pids[k] < 0) {
      perror("my fork failed!");
      exit(EXIT_FAILURE);
      break;
    }
    else if (pids[k] == 0) {
      if (k == 0 && allRunCommands[0].inputfile !=
                        "") {  //If first command, stdin is redirected to input file
        input_fd = open(allRunCommands[0].inputfile.c_str(), O_RDWR);
        if (input_fd == -1) {
          perror("Input file");
          exit(EXIT_FAILURE);
        }
        dup2(input_fd, 0);
      }
      if (k != 0) {  //If not the first command, set its front 0 to read
        dup2(pipe_fds[2 * (k - 1)], 0);
      }
      if (allRunCommands[k].error_outputfile != "") {  //stderr is redirected to error file anywhere
        error_fd = open(allRunCommands[k].error_outputfile.c_str(), (O_RDWR | O_CREAT), 0644);
        if (error_fd == -1) {
          perror("Error output file");
          exit(EXIT_FAILURE);
        }
        dup2(error_fd, 2);
      }
      if (k != pipeSize - 1) {  //If not the last command, set its rear end 1 to write
        dup2(pipe_fds[2 * k + 1], 1);
      }
      if (k == pipeSize - 1 && allRunCommands[k].outputfile !=
                                   "") {  //If last command, stdout is redirected to output file
        output_fd = open(allRunCommands[k].outputfile.c_str(), (O_RDWR | O_CREAT), 0644);
        if (output_fd == -1) {
          perror("Output file");
          exit(EXIT_FAILURE);
        }
        dup2(output_fd, 1);
      }
      for (size_t j = 0; j < 2 * (pipeSize - 1); j++) {  //close pipe fds
        close(pipe_fds[j]);
      }
      size_t n = 0;  //change vector<string> to char** for execve
      char ** argv = new char *[allRunCommands[k].strArgv.size() + 1];
      for (n = 0; n < allRunCommands[k].strArgv.size(); n++) {
        argv[n] = new char[allRunCommands[k].strArgv[n].size() + 1];
        strcpy(argv[n], allRunCommands[k].strArgv[n].c_str());
      }
      argv[allRunCommands[k].strArgv.size()] = NULL;
      int exe_status = execve(argv[0], argv, environ);  //execve
      for (n = 0; n < allRunCommands[k].strArgv.size(); n++) {
        delete argv[n];
      }
      delete[] argv;
      if (exe_status == -1) {
        perror("Execve");
        exit(EXIT_FAILURE);
      }
      if (allRunCommands[0].inputfile != "") {  //close inputfile, outputfile and errorfile fd
        close(input_fd);
      }
      if (allRunCommands[k].error_outputfile != "") {
        close(error_fd);
      }
      if (allRunCommands[k].outputfile != "") {
        close(output_fd);
      }
      exit(EXIT_SUCCESS);
    }
  }
  for (size_t j = 0; j < 2 * (pipeSize - 1); j++) {  //close pipe fds
    close(pipe_fds[j]);
  }
  for (size_t j = 0; j < pipeSize; j++) {  //parent process wait n child processes
    pid_t w = wait(&status);
    if (w == -1) {
      perror("Waitpid problem!");
      exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
      std::cout << "Program exited with status " << WEXITSTATUS(status) << std::endl;
    }
    else if (WIFSIGNALED(status)) {
      std::cout << "Program was killed by signal " << WTERMSIG(status) << std::endl;
    }
  }
  delete[] pids;
  delete[] pipe_fds;
}
