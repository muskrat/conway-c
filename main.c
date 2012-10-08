/* Conway's Game of Life main routine
 *
 * I K Stead, 08-10-2012
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>

#include "conway.h"
#include "display.h"
#include "options.h"

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
