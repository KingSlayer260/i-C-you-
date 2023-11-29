#include <stdio.h>

void print_function( char letter );
int ctr; // defineer ctr als een globale int


int main( void )
{
    char star = '*';
    char dash = '-';
    for (ctr = 0 ; ctr < 10; ctr++ ) // for loop tot 9
    {
        print_function( star );
        print_function( dash );
        printf("\n");
    }
    return 0;
}


void print_function ( char letter )
{
    int test; // defineer test als een lokale int
    for ( test = 0; test < 9; test++) // for loop tot 8
    {
        printf("%c", letter);
    }
}