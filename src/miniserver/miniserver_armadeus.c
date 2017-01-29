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
#include "as_gpio.h"

// COMPLIATION: gcc -Wall -o miniserver miniserver.c /home/florian/Documents/ENSTA_Bretagne/UV5.2/armadeus-master/target/packages/as_devices/c/as_gpio.c /home/florian/Documents/ENSTA_Bretagne/UV5.2/armadeus-master/target/packages/as_devices/c/as_helpers.c

#define LED_GPIO	21
#define TAILLE_MAX 10

int main(int argc, char *argv[])
{

    // mode : armadeus connected

    // var
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char recv_buff[1024];
    time_t ticks;

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

	// Initialisation GPIO driver
	struct as_gpio_device *led;
	led = as_gpio_open("G3", LED_GPIO);
	if(led == NULL)
	{
		printf("Error: can't open gpio\n");
		exit(1);
	}
	as_gpio_set_pin_direction(led,"out");

	// open morse file
	FILE* morse = NULL;
	morse = fopen("morse.txt", "r");

    // listen
    printf("Server is listening...\n");
    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        int bytes = read(connfd, recv_buff, sizeof(recv_buff)-1);

        if (bytes > 0)
        close(connfd);
            syslog(LOG_NOTICE, "Message received: %s", recv_buff);
		
		char* message = recv_buff;		
		int iterateur = 0;
		char carac = "";
		int index = 0;
		// Read all characters of the message
		while(*message != '\0'){
			carac = *message;
			char chaine[TAILLE_MAX] = "";
			// Treatment of a space character
			if(carac == ' '){
				index = 0;
				chaine[0] = ' ';
				chaine[1] = '\0';
			}
			// Number of the line corresponding to the character
			else{
				index = carac;
				if(index>9){
					index = index-54; // Number of the line of the letter
				}
			}
			// Read all lines of the morse.txt file up to the line corresponding to the letter of the message
			int i = 0;
			while(i<index){
				fgets(chaine, TAILLE_MAX, morse); 
				i++;
			}
			message++;
			fseek(morse, 0, SEEK_SET);
			int it = 2; // Morse code begin at the 3rd character of the line of the morse.txt file
			if(chaine[0]!=' '){
				while(chaine[it] != '\n') {
					//printf("%c", chaine[it]);

					if (chaine[it] == '.') //short
					{
						as_gpio_set_pin_value(led,1);
						sleep(0.2);
					}
					else if (chaine[it] == '-') //long
					{
						as_gpio_set_pin_value(led,1);
						sleep(0.4);
					} 
					if (chaine[it+1] == '\n') //End of the morse code of a letter
					{
						as_gpio_set_pin_value(led,0);
						sleep(0.2);
					}
					// Break between the morse code of two letters
					as_gpio_set_pin_value(led,0);
					sleep(0.2);
					it ++;
				}
			}
			//printf("\n");
		}

    }

	as_gpio_close(led);
    close(listenfd);
    closelog();
	fclose(morse);
}
