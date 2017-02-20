#CXXFLAGS=-Wall -Werror -g -std=c++0x -Wmultichar

lab1: lab1.cpp
	g++ $(CXXFLAGS) lab1.cpp `pkg-config --libs ncursesw` -o lab1

clean:
	rm -f a.out

.PHONY: clean
