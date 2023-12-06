#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGEL_EINDE '\n'

FILE* start_leesbestand( char* bestandsnaam )
{
	FILE *bestand;
	char standaard_bestandsnaam[100] = "input.txt";

	printf("\n");
	printf("Voer de bestandsnaam in voor leestoegang.\t");
	printf("[ Voer # in voor standaardbestand \"%s\" ] \n", standaard_bestandsnaam );	
	scanf("%s", bestandsnaam);   
	
	if ( bestandsnaam[0] == '#' ) {
		strcpy(bestandsnaam, standaard_bestandsnaam);	
		printf("Standaard invoerbestand wordt gebruikt\n");		
	}
	
    bestand = fopen(bestandsnaam, "r");
    if (bestand == NULL)
    {
        printf("Kon bestand %s niet openen \n", bestandsnaam);
        exit(0);
    }	
	
	return bestand;
}	

FILE* start_schrijfbestand( char* bestandsnaam )
{
	FILE *bestand;
	char standaard_bestandsnaam[100] = "output.txt";

	printf("\n");	
	printf("Voer de bestandsnaam in voor schrijftoegang.\t");
	printf("[ Voer # in voor standaardbestand \"%s\" ]\n", standaard_bestandsnaam );	
	scanf("%s", bestandsnaam);   
	if ( bestandsnaam[0] == '1' ) {
		strcpy(bestandsnaam, standaard_bestandsnaam);	
		printf("Standaard uitvoerbestand wordt gebruikt\n");
	}
	
    bestand = fopen(bestandsnaam, "w");
    if (bestand == NULL)
    {
        printf("Kon bestand %s niet openen \n", bestandsnaam);
        exit(0);
    }	
	
	return bestand;
}	

int main()
{
    FILE *invoerBestandPtr, *uitvoerBestandPtr;
    char lees_bestandsnaam[100], schrijf_bestandsnaam[100];
	char gelezen_teken, geschreven_teken;
	
	invoerBestandPtr = start_leesbestand( lees_bestandsnaam );	
	uitvoerBestandPtr = start_schrijfbestand( schrijf_bestandsnaam );	
	
	int teller = 0;
	
	gelezen_teken = fgetc( invoerBestandPtr );
	while ( gelezen_teken != EOF ) { 
	
		if ( ( gelezen_teken != REGEL_EINDE ) && ( gelezen_teken != ' ' ) && ( gelezen_teken != '.' )  ) {		
			geschreven_teken = gelezen_teken + 1;
			teller++;
			printf("\nTeller=%d\tTeken=%c", teller, gelezen_teken );
			if ( (int)gelezen_teken < 100 ) { printf(" "); }
			printf("\tTeken_uit=%c", geschreven_teken );
			
		}
		else {						
			geschreven_teken = gelezen_teken;
			if ( gelezen_teken != REGEL_EINDE ) {
				teller++;
				printf("\nTeller=%d\tTeken=%c", teller, gelezen_teken);
			}	
		}	
		fputc( geschreven_teken, uitvoerBestandPtr);		
		gelezen_teken = fgetc( invoerBestandPtr );
	

		if ( gelezen_teken == EOF ) { printf("\nTeller=%d\tTeken=EOF: NIET NAAR BESTAND GESCHREVEN", teller+1 ); }	

    }

    printf("\nInhoud van %s is met 1 ASCII waarde verschoven", lees_bestandsnaam );  
    printf("\nInhoud gekopieerd naar %s", schrijf_bestandsnaam );
	printf("\nTotaal aantal verwerkte tekens = %d\n", teller );
  
    fclose(invoerBestandPtr);
    fclose(uitvoerBestandPtr);
		
    return 0;
}

