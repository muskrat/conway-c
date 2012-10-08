all:
	clang `sdl-config --cflags --libs` -o conway main.c
