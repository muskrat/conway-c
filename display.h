/* Function definitions for conway.c
 *
 * I K Stead, 08-10-2012
 */
#include "display.c"

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void draw_world_pixels(SDL_Surface *surface, int world[MAX_X][MAX_Y]);

void draw_world_rects(SDL_Surface *surface, int world[MAX_X][MAX_Y], int cellsize, int linew);

