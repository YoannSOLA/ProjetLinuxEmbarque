#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include "morse.h"

int len(char* chaine){
    int i;
    i=0;
    while(chaine[i]!='\0'){
        ++i;
    }
    return i;
}

//Fonction aide au cryptage
int vigenereC(int m, int k) {
  int crypted = m + k;
  while(crypted>90)
  {
	crypted-=26;
  }
  return crypted;
}

//Fonction de cryptage
void vigenereCrypt(char *message,char *encrypted_message, char* code){
    int i = 0;
    int j = 0;
    int k = 0;
    while(message[i] != '\0'){
    	//vérifie si on peut parcourir la clé entièrement (en fonction du nombre de pixels restants)
        if ((i+len(code))<=len(message)){ 
            for (j=0; j<len(code); j++){ //parcourt la clé
            	//application du cryptage de vigenere avec un caractère de la clé
                encrypted_message[i+j]=(char)vigenereC((int)message[i+j], (int)code[j]);
		
            }
        }
        else{
            k = len(message)-i;
            for (j=0; j<k; j++){
            	//application du cryptage de vigenere avec un caractère de la clé
                encrypted_message[i+j]=(char)vigenereC((int)message[i+j], (int)code[j]);
		
            }
        }
        i = i+len(code);
  	}
}



int main(int argc, char *argv[])
{
    int sockfd = 0;
    char *send_buff;
    char *message;
    char *encrypted_message=malloc(100*sizeof(char));
    char *code ="auegfusygfus";
    struct sockaddr_in serv_addr;


    // read arguments
    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <msg to send>\n",argv[0]);
        return 1;
    }
    



    // raw message to send
    message = argv[2];
    printf("Message a envoyer : %s\n", message);
    // encryption with vigenere
    vigenereCrypt(message,encrypted_message, code);
    printf("Cryptage par viginere : %s\n", encrypted_message);

    // conversion into morse
    char* sortie_morse = malloc(100*sizeof(char));
    morse(encrypted_message, sortie_morse);
    printf("Codage en morse : %s\n", sortie_morse);


    // buffer to send
    send_buff = encrypted_message;



    // init socket to get a file descriptor
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    // socket configuration to talk on port 5000
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // connect to server
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    int bytes = write(sockfd, send_buff, 100*sizeof(send_buff));

    if(bytes < 0)
    {
        printf("\n Send error \n");
    }

    // close socket
    close(sockfd);

    return 0;
}
