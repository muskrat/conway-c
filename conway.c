/* Conway's Game of Life in C
 *
 * I K Stead, 20-09-2012
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_X 5
#define MAX_Y 5

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
    int x, y, cell, count;
    int this_cell = world[x_pos][y_pos];
    /* Iterate through neighbouring cells */
    for (y = -1; y < 2; y++) {
        for (x = -1; x < 2; x++) {
            cell = world[x_pos + x][y_pos + y];
            /* We don't count the cell we're neighbour-checking */
            if (cell == 1 && cell != this_cell) {
                count += 1;
            }
        }
    }
    return count;
}

/* Advance game one step: apply rules to all cells in world */
void apply_rules(int world[MAX_X][MAX_Y])
{
    int x, y;
    for (y = 0; y < MAX_X; y++) {
        for (x = 0; x < MAX_Y; x++){
            world[x][y] = 0;
        }
    }
}

/* Randomise world state */
void populate_random(int world[MAX_X][MAX_Y])
{
    int x, y;
    for (y = 0; y < MAX_X; y++) {
        for (x = 0; x < MAX_Y; x++){
            world[x][y] = randint();
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
    
    /* Randomise array values */
    populate_random(world);

    int x, y;
    /* Print out array values */
    for (y = 0; y < MAX_X; y++) {
        for (x = 0; x < MAX_Y; x++){
            printf("%i ", world[x][y]);
        }
        printf("\n");
    }
    /* Test neighbour count function */
    int neighbours = count_neighbours(world, 0, 0);
    printf("neighbours: %i \n", neighbours);
    return 0;
}

