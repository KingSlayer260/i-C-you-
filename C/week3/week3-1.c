#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Gebruik: %s <origineel_bestand> <gekopieerd_bestand>\n", argv[0]);
        return 1;
    }

    FILE *origineelBestand, *gekopieerdBestand;
    char karakter;

    // Open het originele bestand voor lezen
    origineelBestand = fopen(argv[1], "r");

    // Controleer of het bestand succesvol is geopend
    if (origineelBestand == NULL) {
        printf("Kan het originele bestand niet openen.\n");
        return 1;
    }

    // Open het gekopieerde bestand voor schrijven
    gekopieerdBestand = fopen(argv[2], "w");

    // Controleer of het bestand succesvol is geopend
    if (gekopieerdBestand == NULL) {
        printf("Kan het gekopieerde bestand niet maken.\n");
        fclose(origineelBestand);
        return 1;
    }

    // Variabele om het aantal karakters bij te houden
    int aantalKarakters = 0;

    // Lees karakters uit het originele bestand en druk inhoud af
    printf("Inhoud van het originele bestand:\n");
    while ((karakter = fgetc(origineelBestand)) != EOF) {
        putchar(karakter);
        // Karakters, zoals LF en EOF, die niet zichtbaar zijn, niet meegeteld
        if (karakter != '\n' && karakter != EOF) {
            aantalKarakters++;
        }
    }
    printf("\n");

    // Terug naar het begin van het bestand voor de volgende loop
    rewind(origineelBestand);

    // Lees karakters uit het originele bestand en schrijf gekopieerde karakters naar het nieuwe bestand
    printf("Inhoud van het gekopieerde bestand (vervangende waardes):\n");
    while ((karakter = fgetc(origineelBestand)) != EOF) {
        // Schrijf het gekopieerde karakter naar het nieuwe bestand
        if (karakter == '\n' || karakter == EOF) {
            putchar(karakter);
            fputc(karakter, gekopieerdBestand);
        } else {
            // Behoud alle karakters inclusief spaties, verschuif alleen andere tekens
            if (karakter != ' ') {
                char vervangendKarakter = karakter + 1;
                putchar(vervangendKarakter);
                fputc(vervangendKarakter, gekopieerdBestand);
            } else {
                putchar(' ');
                fputc(' ', gekopieerdBestand);
            }
        }
    }

    // Sluit beide bestanden
    fclose(origineelBestand);
    fclose(gekopieerdBestand);

    // Print het aantal karakters
    printf("\nAantal zichtbare karakters in het originele bestand: %d\n", aantalKarakters);

    return 0;
}
