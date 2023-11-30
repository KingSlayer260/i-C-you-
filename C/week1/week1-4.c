#include <stdio.h>

// Functie om de omtrek van de cirkel te berekenen
void calculate(double diameter, double *omtrek) {
    // De formule voor de omtrek van een cirkel is omtrek = π * diameter
    *omtrek = 3.14159 * diameter;
}

int main() {
    double diameter;
    double omtrek;

    // Vraag de gebruiker om de diameter in te voeren
    printf("Voer de diameter van de cirkel in: ");
    scanf("%lf", &diameter);

    // Roep de calculate functie aan en geef de pointer mee
    calculate(diameter, &omtrek);

    // Druk het resultaat af
    printf("De omtrek van de cirkel met diameter %.2f is %.2f\n", diameter, omtrek);

    return 0;
}