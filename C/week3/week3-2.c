#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMAAL_INVOEREN 200 // Maxmiaal aantal persoonsgegevens dat ingevoerd kan worden om crashes te voorkomen

// Definieer de Persoon structuur
struct Persoon {
    char naam[50];
    int leeftijd;
};

int main() {
    int aantalPersonen = 0; // Houd het aantal aangemaakte personen bij
    struct Persoon *personen = NULL; // Pointer naar het blok van structuren

    char doorgaan;

    do {
        // Controleren of het maximale aantal personen niet is overschreden
        if (aantalPersonen >= MAXIMAAL_INVOEREN) {
            printf("Het maximale aantal ingevoerde personen is bereikt. Je kan geen nieuwe personen toevoegen.\n");
            break;
        }

        // Verkrijg geheugen voor een extra Persoon structuur
        struct Persoon *tmp = (struct Persoon *)realloc(personen, (aantalPersonen + 1) * sizeof(struct Persoon));

        // Controleer of realloc succesvol was
        if (tmp == NULL) {
            printf("Geheugenfout bij realloc.\n");

            // Geef eerder toegewezen geheugen vrij voordat het programma eindigt
            free(personen);

            exit(EXIT_FAILURE);
        }

        // Wijs de nieuwe pointer toe aan 'personen'
        personen = tmp;

        // Invoeren van gegevens voor de nieuwe Persoon
        printf("Voer de naam in voor persoon %d: ", aantalPersonen + 1);
        scanf("%s", personen[aantalPersonen].naam);

        printf("Voer de leeftijd in voor persoon %d: ", aantalPersonen + 1);
        scanf("%d", &personen[aantalPersonen].leeftijd);

        // Verhoog het aantalPersonen
        aantalPersonen++;

        // Vragen of de gebruiker nog een persoon wil toevoegen
        printf("Wil je nog een persoon toevoegen? (j/n): ");
        scanf(" %c", &doorgaan);

    } while (doorgaan == 'j' || doorgaan == 'J');

    // Print de informatie van alle aangemaakte personen
    printf("\nOverzicht van alle aangemaakte personen:\n");
    for (int i = 0; i < aantalPersonen; i++) {
        printf("Persoon %d\n", i + 1);
        printf("Naam: %s\n", personen[i].naam);
        printf("Leeftijd: %d\n", personen[i].leeftijd);
        printf("\n");
    }

    // Geef het geheugen vrij voordat het programma eindigt
    free(personen);

    return 0;
}
