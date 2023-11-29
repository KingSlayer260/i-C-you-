#include <stdio.h>

// Functie om het maximum van overeenkomstige elementen in twee arrays te vinden
void maxArrays(float *arr1, float *arr2, float *arr3, int lenght){
    for (int i = 0 ; i < lenght; i++) {
        arr3[i] = (arr1[i] > arr2[i]) ? arr1[i] : arr2[i];
    }
}


int main() {
    // Declaratie en initialisatie van arrays
    float array1[] =  {0.7, 3.3, 0.5, 10.3};
    float array2[] = {4.1, 1.5, 0.5, 2.3};
    float array3[4];

    // Afdrukken van de oorspronkelijke arrays
    printf("array 1 is ");
    for ( int i = 0 ; i < 4; i++){
        printf("%.1f\t", array1[i]);

    }
    printf("\n");

    printf("array 2 is ");
    for ( int i = 0 ; i < 4; i++){
        printf("%.1f\t", array2[i]);

    }
    printf("\n");

    // Roep de functie aan om het maximum van overeenkomstige elementen te vinden
    maxArrays(array1, array2, array3, 4);


    // Afdrukken van het resulterende array
    printf("array 3 is ");
    for ( int i = 0 ; i < 4; i++){
        printf("%.1f\t", array3[i]);

    }

    printf("\n");

    return 0;
}