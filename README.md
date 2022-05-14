# CHIP-8
CHIP-8 interpreter for running CHIP-8 applications.

## Installation
To install this interpreter locally please use the makefile provided with the project:
```bash
$ make all
```
This will generate a `main.out` binary in `bin`.
The interpreter will need software to run (otherwise, you'll just see a black screen).
The project includes a few games that can be passed to interpreter via command line args:
```bash
$ ./bin/main.out games/{the-game-you-would-like-to-run}
```

## Controls
CHIP-8 supports 16 keys of input.
This has been mapped to the following keys on a US English keyboard:
* 1-4
* q-r
* a-f
* z-v

Each of the games uses a different set of keys from those listed above.
Determining the controls will be left as an exercise for the user.

## Compiling to WASM
The makefile provided with the project includes a rule for building a CHIP-8 WASM web app.
Make sure that `emcc` is installed on the system, and then run the following:
```bash
$ make wasm
```
This will generate a `wasm-deploy` directory that will have everything needed for using the CHIP-8 HTML viewer.
**Note:** Due to CORS, assets application will need to be served over a web server.
A simple way to get around this for local testing is to run a Python HTTP server using:
```bash
$ python -m http.server --directory wasm-deploy
```
^ Where `python` is python version 3 and the command is executed from the root of the project directory.

## Public Resource
Users interested in playing with the interpreter from their browser that don't want to go through the installation instructions provided above can use the following [site](https://www.hamologist.com/chip-eight/).
