/*
** listener.c -- a datagram sockets "server" demo
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
#include "fonction-receiver.h"
#define MYPORT "4950" // the port users will be connecting to
#define MAXBUFLEN 1000
// get sockaddr, IPv4 or IPv6:

/*
 * Retire le dernier char d'un String
 */

int retireLast(char* A, int i)
{
	A[i-1]='\0';
	return 0;
}
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
/* Fonction qui lit un argument sur la sortie standard et qui renvoie celui-ci.
 * Revoie NULL si la fonction a échoué.
 */
int stInGetName(){
	while(fgets(fileName,TAILLEMAX,STDIN_FILENO)!=NULL){
		printf("%s\n",fileName);
	}
	return 0;
}
int listener()
{
	struct packet recevoir;
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}
	freeaddrinfo(servinfo);
	printf("listener: waiting to recvfrom...\n");
	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, &recevoir, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	printf("listener: got packet from %s\n",
	inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	printf("listener: packet contains \"%d\"\n", recevoir.type);
	close(sockfd);
	return 0;
}