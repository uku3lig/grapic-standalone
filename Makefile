all: build

build: obj/Grapic.o obj/main.o	
	@echo "=== Linking... ==="
	mkdir -p build
	g++ -o build/main obj/Grapic.o obj/main.o -lSDL2 -lSDL2_image -lSDL2_ttf -s

obj/main.o: main.cpp
	@echo "=== Compiling... ==="
	mkdir -p obj
	g++ -MMD -MP -Isrc -I/usr/include/SDL2 -o obj/main.o -MF obj/main.d -c -Wall main.cpp

obj/Grapic.o: src/Grapic.h src/Grapic.cpp
	@echo "=== Compiling Grapic... ==="
	mkdir -p obj
	g++ -MMD -MP -Isrc -I/usr/include/SDL2 -o obj/Grapic.o -MF obj/Grapic.d -c src/Grapic.cpp

download src/Grapic.cpp src/Grapic.h:
	@echo "=== Downloading... ==="
	@python download_grapic.py

clean:
	rm -rf obj/
	rm -rf build/
