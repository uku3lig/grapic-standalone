# grapic-standalone

![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/uku3lig/grapic-standalone?label=grapic-standalone%20size&color=green)
![GitHub repo size](https://img.shields.io/github/repo-size/ucacaxm/grapic?label=grapic%20size&color=red)

**grapic-standalone** is a very simple makefile made to compile apps that use the [Grapic](https://github.com/ucacaxm/grapic) library.

## Dependencies

You need to have git, g++, make, SDL2, SDL2_image and SDL2_ttf installed on your machine.

### Debian-based (`apt`)
`apt install git make gcc libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev`

### Arch-based (`pacman`)
`pacman -S --needed git make gcc sdl2 sdl2_image sdl2_ttf`

## Installation

You can run `bash -c "$(curl -fsSL https://raw.githubusercontent.com/uku3lig/grapic-standalone/master/setup.sh)"` to setup a workspace in the current directory.

Alternatively, you can download [setup.sh](https://raw.githubusercontent.com/uku3lig/grapic-standalone/master/setup.sh) and run `bash setup.sh <PROJECT_NAME>` to download the makefile.

## Usage

Edit `main.cpp` to your liking, then run `make`. It will generate a `main` file in the `build` directory, which you can execute to run your program.

Run `make clean` to remove all build files.

If you use VSCode, you can run `make download` to download the Grapic source files, which will remove the errors in the `#include`.
