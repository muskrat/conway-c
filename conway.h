/* Function declarations for conway.c
 *
 * I K Stead, 08-10-2012
 */
#include "conway.c"


int randint();

int count_neighbours(int world[][MAX_Y], int x_pos, int y_pos);

void apply_rules(int world[MAX_X][MAX_Y]);

void populate(int world[MAX_X][MAX_Y], int rand);

void print_world(int world[MAX_X][MAX_Y]);
