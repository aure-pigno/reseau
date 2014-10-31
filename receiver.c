#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "listener.c"
#define MYPORT "4950"	// the port users will be connecting to

int main(int argc, char** argv){
	if(argc != 5 && argc !=3){
		printf("Erreur sur le nombre d'arguments\n");
		return -1;
	}
	char* fileName = NULL;
	char* hostName = NULL;
	char* port = NULL;
	int i;
	int boolean = 0;
	for(i=1; i<argc;i++)
	{
		if(strcmp("--file",argv[i])==0)
		{
			fileName = argv[i+1];
			i++;
			//printf("filename %s\n", fileName);
		}
		else if (boolean == 0)
		{
			hostName = argv[i];
			//printf("hostname %s\n", hostName);
			boolean = 1;
		}
		else
		{
			port = argv[i];
			//printf("port %s\n", port);
		}
	}
	if (port == NULL || hostName == NULL)
	{
		printf("Erreur arguments\n");
		exit(-1); 
	}


	
	if(listener(fileName, port, hostName)!= 0)
	{
		printf("Erreur listener!");
	}
	return 0;
}


