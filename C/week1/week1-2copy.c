#include <stdio.h>

void print_function ( char letter )
{
    int test; // defineer test als een lokale int
    for ( test = 0; test < 9; test++) // for loop tot 8
    {
        printf("%c", letter);
    }
}