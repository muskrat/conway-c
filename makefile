# Makefile for Conway's Game of Life in C with SDL

all:
	clang `sdl-config --cflags --libs` -o conway conway.c
