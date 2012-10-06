/* Conway's Game of Life in C
 *
 * (c) I K Stead, 20-09-2012
 *
 * First rule: any live cell with fewer than two live
 * neighbours dies of loneliness
 *
 * Second rule: any live cell with two or three live
 * neighbours lives on to the next generation.
 *
 * Third rule: any live cell with more than three live 
 * neighbours dies of overpopulation.
 *
 * Fourth rule: any dead cell with exactly three live
 * neighbours comes to life.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <SDL/SDL.h>

#define ALIVE       1
#define DEAD        0
#define MAX_X       200
#define MAX_Y       200
#define WAIT        100000  /* Microseconds to wait between iterations */
#define CELLSIZE    4       /* Width/height of cells in pixels */
#define LINE        1       /* Pixel width of line between cells */
#define DENSITY     7       /* Starting density of live cells */

/* Generate a random integer between 0 and 1 */
int randint()
{
    int r = rand() % 2;
    return r;
}

/* Choose 0 or 1 at random with weighting */
int weighted_randint(int true_weight)
{
    int choice = rand() % 10;  /* Take last digit of random int */
    
    if (choice > true_weight)
        return 1;
    else
        return 0;
}

/* Count the number of living neighbours of a given cell */
int count_neighbours(int world[MAX_X][MAX_Y], int x_pos, int y_pos)
{
    int x, y, cx, cy, cell;
    int count = 0;
    /* Iterate through neighbouring cells */
    for (y = -1; y < 2; y++) {
        for (x = -1; x < 2; x++) {
            cx = x_pos + x;
            cy = y_pos + y;
            /* Check that current x and y indices aren't out of bounds */
            if ( (0 <= cx && cx < MAX_X) && (0 <= cy && cy < MAX_Y)) {
                cell = world[x_pos + x][y_pos + y];
                if (cell == ALIVE)
                    count ++;
            }
        }
    }
    /* Make sure the cell we're checking isn't counted */
    if (world[x_pos][y_pos] == ALIVE) {
        count --;
    }
    return count;
}

/* Advance game one step: apply rules to all cells in source */
void apply_rules(int world[MAX_X][MAX_Y])
{
    int x, y, cell, neighbours;
    /* Create a temporary copy of world to iterate through. This is so
     * the state of the world isn't changing as it's being checked.
     */
    int temp[MAX_X][MAX_Y];
    memcpy(temp, world, sizeof(temp));

    for (y = 0; y < MAX_X; y++) {
        for (x = 0; x < MAX_Y; x++){
            cell = temp[x][y];
            neighbours = count_neighbours(temp, x, y);
            
            if (cell == ALIVE) {     
                if (neighbours < 2 || neighbours > 3)
                    world[x][y] = DEAD;
            } 
            else { /* Dead */
                if (neighbours == 3)
                    world[x][y] = ALIVE;
            }
        }
    }
}
/* Randomise world state */
void populate(int world[MAX_X][MAX_Y], int rand)
{
    int x, y;
    for (y = 0; y < MAX_X; y++) {
        for (x = 0; x < MAX_Y; x++){
            if (rand == 1)
                world[x][y] = weighted_randint(DENSITY);
            else
                world[x][y] = 0;
        }
    }
}
/* Print world array to terminal, followed by a newline. */
void print_world(int world[MAX_X][MAX_Y])
{
    int x, y;
    char dc;
    /* Print out array values */
    for (y = 0; y < MAX_X; y++) {
        for (x = 0; x < MAX_Y; x++){
            if (world[x][y] == ALIVE)
                dc = '@';
            else 
                dc = '`';
            printf("%c ", dc); 
        }
        printf("\n");
    }
    printf("\n");
}

/* 
 * SDL display code 
 */

#define DEPTH 32    /* Colour depth */

/* Set the pixel at (x, y) to the given value.
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
 * TODO: Add colour selection
 */
void draw_world_rects(SDL_Surface *surface, int world[MAX_X][MAX_Y], int cellsize, int linew)
{
    /* Map colour white to the display */
    Uint32 white = SDL_MapRGB(surface->format, 0xff, 0xff, 0xff);

    int x, y;
    for (y = 0; y < MAX_Y; y++) {
        for (x = 0; x < MAX_X; x++) {
            if (world[x][y] == 1) {
                /* Cell rectangle; Multiply array indices to get top left,
                 * subtract linewidth from dimensions for line between 
                 * cells.
                 */
                SDL_Rect cell = {x*cellsize, y*cellsize, cellsize-linew, cellsize-linew};
                SDL_FillRect(surface, &cell, white);
            }
        }
    }
}

int main()
{
    /* Seed random number generator. Seeded with system time so a
     * different random sequence is produced each time
     */
    srand(time(0));
    
    int world[MAX_X][MAX_Y];
    populate(world, 1); /* Initialise world array with random values */

    /* Set up SDL */
    SDL_Surface *screen;
    SDL_Event event;
    int user_exit;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    /* Initialise screen with size dependent on cell size. TODO: Either
     * remove pixel drawing function or add code to allow easy selection
     * between rect and pixel drawing methods.
     */
    screen = SDL_SetVideoMode(MAX_X*CELLSIZE, MAX_Y*CELLSIZE, DEPTH, SDL_SWSURFACE);

    /* Main loop */
    while (!user_exit) {
        SDL_FillRect(screen, NULL, 0); /* Blank out screen */
        apply_rules(world);
        draw_world_rects(screen, world, CELLSIZE, LINE);
        SDL_Flip(screen);   /* Updates SDL window */
        
        /* Check for user input, quit if required */
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    user_exit = 1;
                    break;
                case SDL_KEYDOWN:
                    user_exit = 1;
                    break;
            }
        }
        usleep(WAIT);
    }
    SDL_Quit();
    return EXIT_SUCCESS;
}
