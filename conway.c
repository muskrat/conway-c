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
                if (cell == 1)
                    count ++;
            }
        }
    }
    /* Make sure the cell we're checking isn't counted */
    if (world[x_pos][y_pos] == 1) {
        count -= 1;
    }
    return count;
}

/* Advance game one step: apply rules to all cells in world */
void apply_rules(int world[MAX_X][MAX_Y], int next[MAX_X][MAX_Y])
{
    int x, y, cell, neighbours;

    for (y = 0; y < MAX_X; y++) {
        for (x = 0; x < MAX_Y; x++){
            cell = world[x][y];
            neighbours = count_neighbours(world, x, y);
            /*
             * First rule: any live cell with fewer than two live
             * neighbours dies of loneliness
             */
            if (cell == 1 && neighbours < 2) {
                next[x][y] = 0;
            }
            /* Second rule: any live cell with two or three live
             * neighbours lives on to the next generation. This 
             * doesn't really need to be here but it makes it more
             * obvious what's going on.
             */
            else if (cell == 1 && neighbours == 2 || neighbours == 3) {
                next[x][y] = 1;
            }
            /* Third rule: any live cell with more than three live 
             * neighbours dies of overpopulation.
             */
            else if (cell == 1 && neighbours > 3) {
                next[x][y] = 0;
            }
            /* Fourth rule: any dead cell with exactly three live
             * neighbours becomes alive
             */
            else if (cell == 0 && neighbours == 3) {
                next[x][y] = 1;
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

void print_world(int world[MAX_X][MAX_Y])
{
    int x, y;
    /* Print out array values */
    for (y = 0; y < MAX_X; y++) {
        for (x = 0; x < MAX_Y; x++){
            printf("%i ", world[x][y]);
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
    int next[MAX_X][MAX_Y];

    populate(world, 1);
    populate(next, 0);
    print_world(world);
    apply_rules(world, next);
    print_world(next);

    return 0;
}

