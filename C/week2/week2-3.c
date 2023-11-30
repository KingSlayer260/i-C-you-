#include <stdio.h>

// Definieer de structuur 'Opleiding'
struct Opleiding {
    char naamOpleiding[40];
    int instroomJaar;
};

// Definieer de structuur 'Student', die de structuur 'Opleiding' bevat
struct Student {
    char naam[40];
    int leeftijd;
    struct Opleiding opleiding;
};

int main() {
    // Declareer een array van studenten
    struct Student studenten[3];

    // Loop om gegevens voor elke student in te voeren
    for (int i = 0; i < 3; i++) {
        printf("Voer de naam van student %d in: ", i + 1);
        scanf("%39s", studenten[i].naam);

        printf("Voer de leeftijd van student %d in: ", i + 1);
        scanf("%d", &studenten[i].leeftijd);

        printf("Voer de naam van de opleiding van student %d in: ", i + 1);
        scanf("%39s", studenten[i].opleiding.naamOpleiding);

        printf("Voer het instroomjaar van student %d in: ", i + 1);
        scanf("%d", &studenten[i].opleiding.instroomJaar);
    }

    // Toon gegevens voor elke student
    for (int i = 0; i < 3; i++) {
        printf("\nGegevens van student %d:\n", i + 1);
        printf("Naam: %s\n", studenten[i].naam);
        printf("Leeftijd: %d\n", studenten[i].leeftijd);
        printf("Opleiding: %s\n", studenten[i].opleiding.naamOpleiding);
        printf("Instroomjaar: %d\n", studenten[i].opleiding.instroomJaar);
    }

    return 0;
}