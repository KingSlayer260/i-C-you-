#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definition of the Persoon structuur
struct Persoon {
    char naam[50];
    int leeftijd;
};

int main() {
    struct Persoon *personen = NULL; // Pointer to the first person
    int aantal_personen = 0;
    char keuze;

    do {
        // Increase memory for an extra person
        aantal_personen++;
        personen = realloc(personen, aantal_personen * sizeof(struct Persoon));

        // Enter data for the new person
        printf("Voer naam in voor persoon %d: ", aantal_personen);
        scanf("%s", personen[aantal_personen - 1].naam);
        
        printf("Voer leeftijd in voor persoon %d: ", aantal_personen);
        scanf("%d", &personen[aantal_personen - 1].leeftijd);

        printf("Wil je nog een persoon toevoegen? (j/n): ");
        scanf(" %c", &keuze); // Note the space before %c to consume any newline

    } while (keuze == 'j' || keuze == 'J');

    printf("\nInformatie van alle toegevoegde personen:\n");
    for (int i = 0; i < aantal_personen; i++) {
        printf("Persoon %d:\n", i + 1);
        printf("Naam: %s\n", personen[i].naam);
        printf("Leeftijd: %d\n\n", personen[i].leeftijd);
    }

    // Free allocated memory
    free(personen);

    return 0;
}
