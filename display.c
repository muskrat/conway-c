/* SDL display code for Conway's Game of Life
 *
 * I K Stead, 08-10-2012
 *
 * TODO: Pass world size as a parameter cf. conway.h todo
 */
#include <SDL/SDL.h>

#include "options.h"

/* Set the pixel at (x, y) to the given colour.
 * NOTE: The surface must be locked before calling this!
 *  From SDL docs - guidevideo.html
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

/* Draw a white pixel to given screen for each live cell in given world
 * array. TODO: Make it easier to change colours.
 */
void draw_world_pixels(SDL_Surface *surface, int world[MAX_X][MAX_Y]) 
{
    /* Map colour white to the display */
    Uint32 white = SDL_MapRGB(surface->format, 0xff, 0xff, 0xff);

    /* Lock the screen for direct access to the pixels */
    SDL_LockSurface(surface);

    /* Loop through world array, drawing pixels to screen */
    int x, y;
    for (y = 0; y < MAX_Y; y++) {
        for (x = 0; x < MAX_X; x++) {
            if (world[x][y] == 1)
                putpixel(surface, x, y, white);
        }
    }
    /* Unlock surface */
    SDL_UnlockSurface(surface);
}

/* Draw a white rectangle of specified size to the screen for each cell
 * in given world array.
 * TODO: Add colour selection and a thin line in between the cells
 */
void draw_world_rects(SDL_Surface *surface, int world[MAX_X][MAX_Y], int cellsize, int linew)
{
    /* Map colour white to the display */
    Uint32 white = SDL_MapRGB(surface->format, 0xff, 0xff, 0xff);

    int x, y;
    for (y = 0; y < MAX_Y; y++) {
        for (x = 0; x < MAX_X; x++) {
            if (world[x][y] == 1) {
                SDL_Rect cell = {x*cellsize, y*cellsize, cellsize-linew, cellsize-linew};
                SDL_FillRect(surface, &cell, white);
            }
        }
    }
}
