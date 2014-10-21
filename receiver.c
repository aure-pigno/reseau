#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fonction-receiver.h"
#define TAILLEMAX 100

int main(int argc, char** argv){
	if(argc != 5 && argc !=3){
		printf("Erreur sur le nombre d'arguments\n");
		return -1;
	}
	char* fileName = NULL;
	char* hostName = NULL;
	int port = -1;
	int i;
	for(i=0; i<argv;i++)
	{
		if(strcmp("[--file",argv[i])==0)
		{
			filename = argv[i+1];
			i++;
			retireLast(filename, length(filename));
			printf("filename %s\n", filename);
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
		if (port == -1 || hostname == NULL)
		{
			printf("Erreur arguments\n");
		exit(-1); 
		}
		else if(fileName == NULL)
		{
			stInGetName()
		}
		

	}
	
	printf("Le nom du fichier est : %s\n",fileName);
	printf("Le nom du host est : %s\n",hostName);
	printf("Le port est : %d\n",port);
	
	int err = listener();
	return 0;
}


