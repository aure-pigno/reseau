/*
** talker.c -- a datagram "client" demo
*/

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
#define SERVERPORT "4950" // the port users will be connecting to
int talker(int argc, char *argv[])
{
	struct packet envoi;
	envoi.type = 1;
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	if (argc != 3) {
		fprintf(stderr,"usage: talker hostname message\n");
		exit(1);
	}
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}
	if ((numbytes = sendto(sockfd, &envoi, sizeof(struct packet), 0,p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}
	freeaddrinfo(servinfo);
	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
	close(sockfd);
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
	if(A[1]>='0' && A[1]>='9')
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/* 
*Récupère la taille d'un fichier
*/
uint32_t get_file_size(char *file_name)
{
	FILE *fp;
	uint32_t file_size;
	file_size = 0;
	if ((fp = fopen(file_name, "rb" )) == NULL) {
		fprintf(stderr, "Cannot open %s.\n", file_name);
		return(file_size);
	}
	if (fseek(fp, (long)(0), SEEK_END) != 0) {
		fclose(fp);
		return(file_size);
	}
	file_size = (uint32_t) (ftell(fp));
	fclose(fp);
	return(file_size);
}
