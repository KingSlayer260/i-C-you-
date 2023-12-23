#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) { // als er geen 3 argumenten zijn gegeven print en return 1. de 3 argumenten horen dus te zijn <naam executable> <origineel_bestand> <gekopieerd_bestand>
        printf("Gebruik: %s <origineel_bestand> <gekopieerd_bestand>\n", argv[0]);
        return 1;
    }

    FILE *origineelBestand, *gekopieerdBestand;
    char karakter;
    int aantalKarakters = 0;

    origineelBestand = fopen(argv[1], "r"); // open origineel bestand
    if (origineelBestand == NULL) {
        printf("Kan het originele bestand niet openen.\n");
        return 1;
    }

    gekopieerdBestand = fopen(argv[2], "w"); // op gekopierd bestand
    if (gekopieerdBestand == NULL) {
        printf("Kan het gekopieerde bestand niet maken.\n");
        fclose(origineelBestand);
        return 1;
    }

    printf("Inhoud van het gekopieerde bestand:\n");
    while ((karakter = fgetc(origineelBestand)) != EOF) {
        if (karakter != '\n' && karakter != EOF) {
            aantalKarakters++; // telt 1 op voor elke karakter dat geen spatie is
        }

        // Verwerk het karakter voor het gekopieerde bestand
        char vervangendKarakter;
        if (karakter == '\n' || karakter == ' ') {
            vervangendKarakter = karakter;
        } else {
            vervangendKarakter = karakter + 1;
        }
        
        fputc(vervangendKarakter, gekopieerdBestand); // kopieerd naar het nieuwe bestand
        putchar(vervangendKarakter); //print de gelezen karakter
    }

    printf("\nAantal zichtbare karakters in het originele bestand (exclusief spaties): %d\n", aantalKarakters);

    fclose(origineelBestand); // schoonmaken
    fclose(gekopieerdBestand);

    return 0;
}






