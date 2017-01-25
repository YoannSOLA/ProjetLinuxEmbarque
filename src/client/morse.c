#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "morse.h"


void morse(char message[],char* sortie){
	
	

	FILE* morse = NULL;
	morse = fopen("morse.txt", "r");

	int iterateur = 0;
	char carac = '\0';
	int index = 0;

	while(message[iterateur] != '\0') {
		
 		carac = message[iterateur];
		char chaine[TAILLE_MAX] = "";
				
		if(carac == ' '){
			index = 0;
			chaine[0] = ' ';
			chaine[1] = '\0';
		}
		else{
			index = carac;
			if(index>9){
				index = index-54;
			}
		}
		int i = 0;
		while(i<index){
			fgets(chaine, TAILLE_MAX, morse);
			i++;
		}
 		iterateur++;
		fseek(morse, 0, SEEK_SET);
		int it = 2;
		if(chaine[0]!=' '){
			while(chaine[it] != '\n') {
				*sortie=chaine[it];
				sortie++;

				it ++;
				if(chaine[it] == '\n'){
					*sortie=' ';
					sortie++;
				}
			}
		}
		else{
			
		}
	}
	fclose(morse);


}

/*
int main(int argc, char *argv[])
{
	char* sortie_crypte = malloc(100);
	char message[] = {"MON VIER"};
	morse(message, sortie_crypte);
	printf("%s", sortie_crypte);

    return 0;
}*/
