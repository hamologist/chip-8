CC := clang++

CFLAGS := -std=c++0x
LIB := 
TESTLIB := -lgtest -lpthread
INC := -I include

all: bin build bin/main.out

test: test/main.out

bin:
	mkdir -p bin

build:
	mkdir -p build

# Main Code 
bin/main.out: build/main.o build/chip_eight.o
	$(CC) $(CFLAGS) build/chip_eight.o build/main.o -o bin/main.out $(LIB)

build/chip_eight.o: src/chip_eight.cpp
	$(CC) $(CFLAGS) $(INC) -c src/chip_eight.cpp -o build/chip_eight.o

build/main.o: src/main.cpp
	$(CC) $(CFLAGS) $(INC) -c src/main.cpp -o build/main.o

# Test Code
test/main.out: test/main.o build/chip_eight.o
	$(CC) $(CFLAGS) build/chip_eight.o test/main.o -o test/main.out $(TESTLIB)

test/main.o: test/main.cpp
	$(CC) $(CFLAGS) $(INC) -c test/main.cpp -o test/main.o

clean:
	rm -rf bin build

