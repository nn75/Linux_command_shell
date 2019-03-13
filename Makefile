myShell: myParse.h myShell.h runCommand.h myCommand.h myExport.h mySet.h myCd.h myInc.h  main.cpp
	g++ -o myShell -ggdb3 -Wall -Werror -pedantic -std=gnu++98 main.cpp runCommand.cpp myParse.cpp myShell.cpp myCommand.cpp mySet.cpp myCd.cpp myExport.cpp myInc.cpp

clean:
	rm -f *~ *.o
