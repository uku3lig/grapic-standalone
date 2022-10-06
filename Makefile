all: build

build: src/Grapic.cpp main.cpp
	@echo "=== Compiling... ==="
	mkdir -p obj
	g++ -MMD -MP -Isrc -I/usr/include/SDL2 -o obj/Grapic.o -MF obj/Grapic.d -c src/Grapic.cpp
	g++ -MMD -MP -Isrc -I/usr/include/SDL2 -o obj/main_start.o -MF obj/main_start.d -c -Wall main.cpp
	
	@echo "=== Linking... ==="
	mkdir -p build
	g++ -o build/main obj/Grapic.o obj/main_start.o -lSDL2 -lSDL2_image -lSDL2_ttf -s

src/Grapic.cpp:
	@echo "=== Downloading... ==="
	@python download_grapic.py

clean:
	rm -rf obj/
	rm -rf build/
