/* Little program to test random number functions
 *
 * I K Stead, 21-09-2012
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randint()
{
    int r;
    r = rand() % 2;
    return r;
}

int main()
{
    srand(time(0));
    printf("%i\n", randint());
    return 1;
}
