CXXFLAGS=-Wall -g -std=c++0x -fpermissive

lab1: lab1.cpp
	g++ $(CXXFLAGS) lab1.cpp `pkg-config --libs ncursesw` -o lab1

lab2_1: lab2_1.cpp
	g++ $(CXXFLAGS) lab2_1.cpp `pkg-config --libs ncursesw` -o lab2_1

lab2_2: lab2_2.cpp
	g++ $(CXXFLAGS) lab2_2.cpp -o lab2_2

lab3: lab3.cpp
	g++ $(CXXFLAGS) lab3.cpp -o lab3

lab4: lab4.cpp
	g++ $(CXXFLAGS) lab4.cpp -o lab4

lab6: lab6.cpp
	g++ $(CXXFLAGS) lab6.cpp -o lab6

clean:
	rm -f lab1 lab2_1 lab2_2 lab3 lab4 lab6

.PHONY: clean
