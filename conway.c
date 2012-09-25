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
#include <SFML/System.h>

#define ALIVE   1
#define DEAD    0
#define MAX_X   20
#define MAX_Y   20
#define WAIT    1

/* Generate a random integer between 0 and 1 */
int randint()
{
    int r;
    r = rand() % 2;
    return r;
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
            if (world[x][y] == ALIVE) {
                dc = '@';
            } else {
                dc = '`';
            }
            printf("%c ", dc); 
        }
        printf("\n");
    }
    printf("\n");
}
int main()
{
    /* Seed random number generator. Seeded with system time so a
     * different random sequence is produced each time
     */
    srand(time(0));
    
    int world[MAX_X][MAX_Y];
    populate(world, 1); /* Initialise world array with random values */

    /* Main loop */
    int count = 0;
    char input;

    while (1) {
        apply_rules(world);
        print_world(world);
        sleep(WAIT);
    }
    return EXIT_SUCCESS;
}
