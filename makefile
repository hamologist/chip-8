CC := clang++

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CFLAGS := -std=c++0x
LIB := 
INC := -I include
TESTDIR := lib/googletest/
TESTLIB := -L$(TESTDIR) -lgtest -lpthread

all: bin build bin/main.out

test: bin build test/googletest bin/test_main.out

bin:
	mkdir -p bin

build:
	mkdir -p build

lib/googletest:
	mkdir -p lib/googletest

# Main Code 
bin/main.out: build/main.o build/chip_eight.o
	$(CC) $(CFLAGS) build/chip_eight.o build/main.o -o bin/main.out $(LIB)

build/chip_eight.o: src/chip_eight.cpp
	$(CC) $(CFLAGS) $(INC) -c src/chip_eight.cpp -o build/chip_eight.o

build/main.o: src/main.cpp
	$(CC) $(CFLAGS) $(INC) -c src/main.cpp -o build/main.o

# Test Code
test/googletest: lib/googletest
	cd test; \
	git clone --depth 1 --branch release-1.7.0 https://github.com/google/googletest.git; \
	cd googletest; \
	cmake CMakeLists.txt; \
	make; \
	cp libgtest_main.a libgtest.a $(ROOT_DIR)/lib/googletest; \
	cd include; \
	cp -r gtest/ $(ROOT_DIR)/include;

bin/test_main.out: build/test_main.o build/chip_eight.o
	$(CC) $(CFLAGS) build/chip_eight.o build/test_main.o -o bin/test_main.out $(TESTLIB)

build/test_main.o: test/test_main.cpp
	$(CC) $(CFLAGS) $(INC) -c test/test_main.cpp -o build/test_main.o

clean:
	rm -rf bin build test/googletest include/gtest lib/googletest
