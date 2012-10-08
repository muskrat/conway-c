/* Conway's Game of Life C implementation
 *
 * I K Stead 08-10-2012
 *
 * TODO: Explicitly pass x and y dimensions of world array to functions 
 * to allow user to specify the size of the worl without recompiling
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "options.h"

/* Generate a random integer between 0 and 1 */
int randint()
{
    int r = rand() % 2;
    return r;
}

/* Count the number of living neighbours of a given cell */
int count_neighbours(int world[][MAX_Y], int x_pos, int y_pos)
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
                world[x][y] = randint();
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
