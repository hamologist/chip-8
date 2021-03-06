CC := clang++
EMCC := emcc

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CFLAGS := -std=c++0x -g
EMCCFLAGS := -std=c++11 -O3
LIB := -L/usr/local/lib -lSDL2
INC := -I include -I /usr/local/include
TESTDIR := lib/googletest/
TESTLIB := -L$(TESTDIR) -lgtest -lpthread

all: bin build bin/main.out

wasm: \
	wasm-deploy \
	build \
	wasm-deploy/chip-eight.js \
	wasm-deploy/chip-eight-interface.js \
	wasm-deploy/chip-eight.css \
	wasm-deploy/index.html

test: bin build test/googletest test/test_main.out

bin:
	mkdir -p bin

build:
	mkdir -p build

lib/googletest:
	mkdir -p lib/googletest

wasm-deploy: 
	mkdir -p wasm-deploy

# Main Code 
bin/main.out: build/main.o build/chip_eight.o build/chip_eight_display.o build/chip_eight_controller.o
	$(CC) $(CFLAGS) build/chip_eight.o build/chip_eight_display.o build/chip_eight_controller.o build/main.o -o bin/main.out $(LIB)

build/chip_eight.o: src/chip_eight.cpp
	$(CC) $(CFLAGS) $(INC) -c src/chip_eight.cpp -o build/chip_eight.o

build/chip_eight_display.o: src/chip_eight_display.cpp
	$(CC) $(CFLAGS) $(INC) -c src/chip_eight_display.cpp -o build/chip_eight_display.o

build/chip_eight_controller.o: src/chip_eight_controller.cpp
	$(CC) $(CFLAGS) $(INC) -c src/chip_eight_controller.cpp -o build/chip_eight_controller.o

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
	cp -r gtest $(ROOT_DIR)/include;

test/test_main.out: build/test_main.o build/chip_eight.o
	$(CC) $(CFLAGS) build/chip_eight.o build/test_main.o -o test/test_main.out $(TESTLIB)

build/test_main.o: test/test_main.cpp
	$(CC) $(CFLAGS) $(INC) -c test/test_main.cpp -o build/test_main.o

# Wasm
wasm-deploy/chip-eight.js: build/chip_eight.bc build/chip_eight_embind.bc
	$(EMCC) $(EMCCFLAGS) $(INC) \
		--bind -s WASM=1 \
		build/chip_eight.bc \
		build/chip_eight_embind.bc \
		-o wasm-deploy/chip-eight.js \
		--preload-file games

wasm-deploy/chip-eight-interface.js:
	cp wasm/chip-eight-interface.js wasm-deploy/chip-eight-interface.js

wasm-deploy/chip-eight.css:
	cp wasm/chip-eight.css wasm-deploy/chip-eight.css

wasm-deploy/index.html:
	cp wasm/index.html wasm-deploy/index.html

build/chip_eight.bc: src/chip_eight.cpp
	$(EMCC) $(EMCCFLAGS) $(INC) -c src/chip_eight.cpp -o build/chip_eight.bc

build/chip_eight_embind.bc: src/chip_eight_embind.cpp
	$(EMCC) $(EMCCFLAGS) $(INC) -c src/chip_eight_embind.cpp -o build/chip_eight_embind.bc

# Other
clean:
	rm -rf bin build

clean-all:
	rm -rf bin build include/gtest lib/googletest

tags: src/* include/*
	$(CC) $(CFLAGS) $(INC) -M src/* | sed -e 's/[\\ ]/\n/g' | \
        sed -e '/^$$/d' -e '/\.o:[ \t]*$$/d' | \
        ctags -L - --c++-kinds=+p --fields=+iaS --extra=+q
