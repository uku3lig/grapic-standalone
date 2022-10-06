# grapic-standalone

![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/uku3lig/grapic-standalone?label=standalone%20size) ![GitHub repo size](https://img.shields.io/github/repo-size/ucacaxm/grapic?label=grapic%20size)

**grapic-standalone** is a very simple makefile made to compile apps that use the [Grapic](https://github.com/ucacaxm/grapic) library.

## Dependencies

You need to have g++, make, SDL2, SDL2_image and SDL2_ttf installed on your machine.

To download and compile Grapic itself, you will need Python 3. If you don't want to install it, you can manually download the .cpp and .h files [here](https://github.com/ucacaxm/grapic/tree/master/src) and put them in the `src` directory.

### Debian-based (`apt`)
`apt install make g++ libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev python3`

### Arch-based (`pacman`)
`pacman -S make g++ sdl2 sdl2_image sdl2_ttf python`

## Usage

Clone the repo, edit `main.cpp` to your liking, then run `make`. It will generate a `main` file in the `build` directory, which you can execute to run your program.

Run `make clean` to remove all build files.

If you use VSCode, you can run `make download` to download the Grapic source files, which will remove the errors in the `#include`.
