
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
#include "talker.c"

int convertInt(char* A, int i);
int length(char* A);
uint32_t get_file_size(char *file_name);
/*
 * Donne la longueur du String
 */
int length(char* A)
{
	int i = 0;
	while(A[i] != '\0')
	{
		i++;
	}
	return i;
}
/*
 * Converti un String en int
 */
int convertInt(char* A, int i)
{
	int j;
	int k = 0;
	for(j=0;j<i;j++)
	{
		k = (10*k) + (A[j]-48);
	}
	return k;
}

int main ( int argc, char** argv )
{
	char* filename = NULL;
	char* hostname = NULL;
	int sber = 0;
	int splr = 0;
	int delay = -1;
	char* port = NULL;
	int i;
	int boolean =0;
	for (i=1;i<argc;i++)
	{
		if(strcmp("--file",argv[i])==0)
		{
			filename = argv[i+1];
			i++;
			//printf("filename %s\n", filename);
		}
		else if(strcmp("--sber",argv[i])==0)
		{
			char * a = argv[i+1];
			i++;
			sber = convertInt(a,length(a));
			//printf("sber %d\n", sber);
		}
		else if(strcmp("--splr",argv[i])==0)
		{
			char * a = argv[i+1];
			i++;
			splr = convertInt(a,length(a));
			//printf("splr %d\n", splr);
		}
		else if(strcmp("--delay",argv[i])==0)
		{
			char * a = argv[i+1];
			i++;
			delay = convertInt(a,length(a));
			//printf("delay %d\n", delay);
		}
		else if (boolean == 0)
		{
			hostname = argv[i];
			boolean =1;
			//printf("hostname %s\n", hostname);
		}
		else
		{
			port = argv[i];
			//printf("port %d\n", port);
		}
	}
	if(hostname == NULL || sber < 0 || splr < 0 || delay == -1 || port == NULL)
	{
		printf("Erreur arguments\n");
		exit(-1);
	}
	

	if(talker(filename, port, hostname, delay, sber, splr)!=0)
	{
		printf("Erreur talker\n");
	}
	printf("Le fichier de base contient %d caracteres\n",(int)file_size(filename));
	
	return 0;
}
