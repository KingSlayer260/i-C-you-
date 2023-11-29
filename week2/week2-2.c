#include <stdio.h>

int main() {
    char input[1000]; // Veronderstel dat de zin niet langer zal zijn dan 1000 tekens

    // Vraag de gebruiker om een zin in te voeren
    printf("Voer een zin in: ");

    // Lees de zin in, karakter voor karakter, totdat er een nieuwe regel of EOF wordt bereikt
    int c;
    int length = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (length < sizeof(input) - 1) {
            input[length] = c;
            length++;
        }
    }

    // Voeg een null-terminatiekarakter toe aan het einde van de ingevoerde zin
    if (length < sizeof(input)) {
        input[length] = '\0';
    }

    // Druk de lengte van de ingevoerde zin af
    printf("De ingevoerde zin heeft %d tekens.\n", length);

    return 0;
}