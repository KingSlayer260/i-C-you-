#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definieer de Persoon structuur
struct Persoon {
    char naam[50];
    int leeftijd;
};

void printPersonen(struct Persoon *personen, int aantalPersonen) {
    printf("\nOverzicht van alle aangemaakte personen:\n");
    for (int i = 0; i < aantalPersonen; i++) {
        printf("Persoon %d\n", i + 1);
        printf("Naam: %s\n", personen[i].naam);
        printf("Leeftijd: %d\n", personen[i].leeftijd);
        printf("\n");
    }
}

int main() {
    int aantalPersonen = 0;
    struct Persoon *personen = NULL;
    char doorgaan;

    do {
        struct Persoon *tmp = (struct Persoon *)realloc(personen, (aantalPersonen + 1) * sizeof(struct Persoon));

        if (tmp == NULL) {
            printf("Geheugenfout bij realloc. Het geheugen is vol.\n");

            // Print de reeds ingevoerde personen
            printPersonen(personen, aantalPersonen);

            // Geef eerder toegewezen geheugen vrij
            free(personen);

            return EXIT_FAILURE;
        }

        personen = tmp; 

        printf("Voer de naam in voor persoon %d: ", aantalPersonen + 1);
        scanf("%49s", personen[aantalPersonen].naam); // Gebruik %49s om bufferoverloop te voorkomen

        int inputValid = 0;
        while (!inputValid) { // check of de waarde voor leeftijd wel een int is
            printf("Voer de leeftijd in voor persoon %d: ", aantalPersonen + 1);
            if (scanf("%d", &personen[aantalPersonen].leeftijd) == 1) {
                inputValid = 1;
            } else {
                printf("Ongeldige invoer. Voer een geheel getal in.\n");
                while (getchar() != '\n'); // Leeg de input buffer
            }
        }

        aantalPersonen++;

        printf("Wil je nog een persoon toevoegen? (j/n): ");
        scanf(" %c", &doorgaan);

    } while (doorgaan == 'j' || doorgaan == 'J'); // als invoer j is of J doorgaan

    printPersonen(personen, aantalPersonen); // print lijst met personen

    free(personen); // maak geheugen vrij

    return 0;
}
