all: bin build bin/main.out

bin:
	mkdir -p bin

build:
	mkdir -p build

bin/main.out: build/main.o
	g++ -std=c++0x build/main.o -o bin/main.out

build/main.o: src/main.cpp
	g++ -std=c++0x -c src/main.cpp -o build/main.o

