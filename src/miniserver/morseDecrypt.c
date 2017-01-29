#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "morseDecrypt.h"


void morseDecrypt(char message[],char* sortie){

	FILE* morse = NULL;
	morse = fopen("morse.txt", "r");

	char *caracCrypte=NULL;
	int k=0;

	char *ligne=malloc(100*sizeof(char));

	int decrypt=0;
	
	caracCrypte = strtok (message," ");
	
	while (caracCrypte != NULL)
    	{
    		
		while(decrypt == 0) {
			
 			fgets(ligne, TAILLE_MAX, morse);
			
			char ligne1 = ligne[0];
			char *ligne2=malloc(100*sizeof(char));
			int i=2;
			int j = 0;
			while(ligne[i] != '\n'){
				ligne2[j]=ligne[i];
				i++;
				j++;
			}
			
			if(strcmp( caracCrypte, ligne2 ) == 0){
				decrypt=1;
				sortie[k]=ligne1;
			}

			free(ligne2);

		}
		
		k++;
		decrypt=0;
		fseek(morse, 0, SEEK_SET);
		caracCrypte = strtok(NULL, " ");
	}

	free(ligne);

	fclose(morse);


}

/*
int main(int argc, char *argv[])
{
	char* sortie_crypte = malloc(100*sizeof(char));
	char message[] = "- . ... -";
	printf("Avant : %s\n",message);
	morseDecrypt(message, sortie_crypte);
	printf("Après : %s\n", sortie_crypte);

    return 0;
} */
