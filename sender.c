#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
int retireLast(char* A, int i);
int convertInt(char* A, int i);
int length(char* A);
int isInt(char* A);
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
		exit(-1); 
	}
	return 0;
}
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
 * Retire le dernier char d'un String
 */
int retireLast(char* A, int i)
{
	A[i-1]='\0';
	return 0;
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
/*
 * Verifie si un String est un int
 */
int isInt(char* A)
 {
 	if(A[1]>='0' &&  A[1]>='9')
 	{
 		return 0;
 	}
 	else 
 	{
 		return 1;
 	}
 }

