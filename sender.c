
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "struct.h"
#include "fonction-sender.h"
int retireLast(char* A, int i);
int convertInt(char* A, int i);
int length(char* A);
int isInt(char* A);
uint32_t get_file_size(char *file_name);
int main ( int argc, char** argv )
{
	char* filename = NULL;
	char* hostname = NULL;
	int sber = -1;
	int splr = -1;
	int delay = -1;
	int port = -1;
	int i;
	for (i=1;i<argc;i++)
	{
		if(strcmp("[--file",argv[i])==0)
		{
			filename = argv[i+1];
			i++;
			retireLast(filename, length(filename));
			printf("filename %s\n", filename);
		}
		else if(strcmp("[--sber",argv[i])==0)
		{
			char * a = argv[i+1];
			i++;
			retireLast(a, length(a));
			sber = convertInt(a,length(a));
			printf("sber %d\n", sber);
		}
		else if(strcmp("[--splr",argv[i])==0)
		{
			char * a = argv[i+1];
			i++;
			retireLast(a, length(a));
			splr = convertInt(a,length(a));
			printf("splr %d\n", splr);
		}
		else if(strcmp("[--delay",argv[i])==0)
		{
			char * a = argv[i+1];
			i++;
			retireLast(a, length(a));
			delay = convertInt(a,length(a));
			printf("delay %d\n", delay);
		}
		else if(isInt(argv[i])!=0)
		{
			port = convertInt(argv[i], length(argv[i]));
			printf("port %d\n", port);
		}
		else
		{
			hostname = argv[i];
			printf("hostname %s\n", hostname);
		}
	}
	if(hostname == NULL || sber == -1 || splr == -1 || delay == -1 || port == -1)
	{
		printf("Erreur arguments\n");
		printf("Taille du fichier : %d\n",get_file_size("text.txt"));
		exit(-1);
	}
	char *toSend[2];
	*toSend[1]="localhost";
	*toSend[2]="blabla";
	int err = talker(2, *toSend)
	return 0;
}
