create:  
	g++ `pkg-config --cflags sdl2` -o main.out main.cpp `pkg-config --libs sdl2` -lSDL2_image -lSDL2_ttf -std=c++11