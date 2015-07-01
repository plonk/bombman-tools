all: showmap showmap.exe

showmap: picojson.h main.cpp
	g++ -std=c++11 -Wall -o showmap main.cpp

showmap.exe: picojson.h main.cpp
	x86_64-w64-mingw32-g++ -static -std=c++11 main.cpp -o showmap.exe

clean:
	rm -f showmap showmap.exe *.o
