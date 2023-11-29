#include <stdio.h>
#include "week1-2copy.h"

int main() {
    int keuze;
    char letter = '*'; // Example pointer
    while (1) { // Oneindige lus totdat een negatief getal wordt ingevoerd
        printf("Kies een optie (1, 2, 3) of voer een negatief getal in om te stoppen: ");
        scanf("%d", &keuze); // laat de gebruiker een keuze maken

        if (keuze < 0) {
            printf("Negatief getal ingevoerd. Het programma wordt gestopt.\n");
            break; // Stop de lus als een negatief getal is ingevoerd
        }

        switch (keuze) {
            case 1:
                printf("Optie 1 gekozen.\n");
                break;
            case 2:
                printf("Optie 2 gekozen. Uitvoeren van code uit optie2.c:\n");
                print_function(letter) ;// Roep de code uit week1-2.c aan
                break;
            case 3:
                printf("Optie 3 gekozen.\n");
                break;
            default:
                printf("Ongeldige optie. Kies opnieuw.\n");
                break;
        }
    }

    return 0;
}
