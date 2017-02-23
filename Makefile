CXXFLAGS=-Wall -Werror -g -std=c++0x
# -Wmultichar

lab1: lab1.cpp
	g++ $(CXXFLAGS) lab1.cpp `pkg-config --libs ncursesw` -o lab1

lab2_1: lab2_1.cpp
	g++ $(CXXFLAGS) lab2_1.cpp `pkg-config --libs ncursesw` -o lab2_1

lab2_2: lab2_2.cpp
	g++ $(CXXFLAGS) lab2_2.cpp `pkg-config --libs ncursesw` -o lab2_2

clean:
	rm -f lab1 lab2_1 lab2_2

.PHONY: clean
