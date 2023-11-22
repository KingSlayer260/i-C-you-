#include <stdio.h>
#include "opdracht2.h"
 // Functie prototype voor de code in optie2.c

int main() {
    int keuze;

    while (1) { // Oneindige lus totdat een negatief getal wordt ingevoerd
        printf("Kies een optie (1, 2, 3) of voer een negatief getal in om te stoppen: ");
        scanf("%d", &keuze);

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
                int line(void) ;
                print_function() ;// Roep de code uit optie2.c aan
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
