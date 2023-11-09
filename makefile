all: gatorLibrary.cpp
	g++ gatorLibrary.cpp -o gatorLibrary

run: gatorLibrary
	./gatorLibrary ./testcase-1.txt