#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <string.h>

int len(char* chaine){
    int i;
    i=0;
    while(chaine[i]!='\0'){
        ++i;
    }
    return i;
}

//Fonction aide au décryptage
int vigenereD(int m, int k) {
  int crypted = m - k;
  while(crypted<65)
  {
	crypted+=26;
  }
  return crypted;
}

//Fonction de décryptage
void vigenereDecript(char *encrypted_message, char *message, char* code){
    int i = 0;
    int j = 0;
    int k = 0;
    while(encrypted_message[i] != '\0'){
    	//vérifie si on peut parcourir la clé entièrement (en fonction du nombre de pixels restants)
        if ((i+len(code))<=len(encrypted_message)){ 
            for (j=0; j<len(code); j++){ //parcourt la clé
            	//application du cryptage de vigenere avec un caractère de la clé
                message[i+j]=(char)vigenereD((int)encrypted_message[i+j], (int)code[j]);
		
            }
        }
        else{
            k = len(encrypted_message)-i;
            for (j=0; j<k; j++){
            	//application du cryptage de vigenere avec un caractère de la clé
                message[i+j]=(char)vigenereD((int)encrypted_message[i+j], (int)code[j]);
		
            }
        }
        i = i+len(code);
  	}
}

int main(int argc, char *argv[])
{
    // var
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char recv_buff[1024];
    time_t ticks;

    struct sockaddr_in adr, adresse;
    socklen_t lgadresse=0;

    char *code ="auegfusygfus";
    char *encrypted_message=malloc(100*sizeof(char));
    char *message=malloc(100*sizeof(char));

    memset(recv_buff, '0', sizeof(recv_buff));

    // init syslog
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog ("miniserver", LOG_CONS|LOG_PID|LOG_NDELAY, LOG_LOCAL1);

    // init socket to get a file descriptor
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // configure server to listen message comming from in port 5000
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    // bind and listen
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    // listen
    printf("Server is listening...\n");
    while (1)
    {
        

        connfd = accept(listenfd, (struct sockaddr*)&adresse, &lgadresse);
        int bytes = read(connfd, recv_buff, sizeof(recv_buff)-1);

        if (bytes > 0)
        close(connfd);
        

	int i =0;
        while(recv_buff[i]!='\0'){
		encrypted_message[i]=recv_buff[i];		
		i++;
        }
	
	// message receive
	printf("Message reçu : %s\n",encrypted_message);

	// vigenere
	vigenereDecript(encrypted_message,message,code);
	printf("Decryptage par viginere : %s\n",message);

        fflush(stdout);
        syslog(LOG_NOTICE, "Message received: %s", encrypted_message);

    }

    close(listenfd);
    closelog();
}
