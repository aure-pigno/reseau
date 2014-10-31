#include <time.h>
#include "writeFile.c"


#define nbrStruct 32
#define TYPE 1
#define TAILLE 512
#define min(a,b) (a<=b?a:b)
#define max(a,b) (a>=b?a:b)

/* Fonction talker
 * @author : Martin Bontemps & Aurelien Pignolet
 * @date : 28 Octobre 2014
 * 
 * Cette fonction prend en argument le nom d'un fichier, le port auquel se connecter, l'adresse IP auquelle se connecter
 * ainsi que 3 autres arguments simulant les erreurs de transmission pour vérifier la validite du protocole :
 * - delay qui represente le temps de transmission par cable. (en millisecondes)
 * - sber qui represente le nombre de packets corrompus sur 1000 packets envoyés.
 * - splr qui represent un pourcentage de packet perdu en court de route.
 *
 * @pre : si filename == NULL alors entree sur stdin ; delay >= 0 ; sber >= 0 ; splr >=0
 */

int talker(char * filename, const char * port, const char * hostName, int delay, int sber, int splr)
{	
	/* nbrStructTot represente le nombre de structure 'struct' totale neccessaire pour transmettre
	 * toutes les donnees. Si le nombre de donnees a transmettre est exactement un multiple de 512,
	 * une structure vide est ajoutee pour specifier au receiver que c'est bien la bien du fichier.
	 */
    int nbrStructTot=0;
    
	if (filename != NULL){ 
		long taille_fichier = file_size(filename);
   		nbrStructTot = ((taille_fichier-1)/TAILLE)+1;
   	}
   	
	struct packet * ack = (struct packet*)malloc(sizeof(struct packet)); 
	ack->window = -1;
	// Initialisation d'un pointeur qui servira e receuillir les acknowledgments.
	
	struct packet *tab_struct [nbrStruct];
	// Pointeur vers un tableau de structure ou sera stocké les donnees a envoyer.
	
	int select_result;
	fd_set read_ack;
	struct timeval timeout;
    // Select init.
    
	int sockfd;
	struct addrinfo hints, *p;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // mettre a AF_UNSPEC pour IPv4 et AF_INET pour IPv6
	hints.ai_socktype = SOCK_DGRAM;
	
	if ((rv = getaddrinfo(hostName, port, &hints, &p)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
		perror("talker: socket");
	}
	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}
	freeaddrinfo(p);
	// Socket init.
	
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	addr_len = sizeof their_addr;
	// Pour recevoir l'adresse du recepteur plus tard. Pas utile ici.	
	
	FILE* fp = NULL;
	if(filename == NULL)
	{
		fp = stdin; // Par defaut.
	}
	else if ((fp = fopen(filename, "rb" )) == NULL) 
	{
          fprintf(stderr, "Cannot open %s.\n", filename);
          return(-1);
    }
    // Initialisation du file descriptor.
	
	int boolean; // Prend la valeur 1 si on a envoyé completement une fenetre.
	int i; // Represente le nombre de tableau necessaire pour envoyer toutes les donnees.
	int j;
	int str; // Represente le nombre de structures a envoyer pour une fenetre particuliere.

	for(i=0; i<((nbrStructTot/(nbrStruct))+1); i++)
	{
		boolean = 0; // On le (re)met a zéro pour la nouvelle fenetre.
		
		str = max(0,min((nbrStructTot-(i*(nbrStruct))),nbrStruct)); // Histoire d'être sur qu'il ne soit pas negatif.
		
		int c = 0;// numero de sequence du dernier ack recu.
		
		while(boolean == 0){ // Tant que tout les ack ne sont pas recu.
		
			timeout.tv_sec = 0; /* 0 s */
    		timeout.tv_usec = 500*100; /* 500 ms */
    		
    		FD_ZERO(&read_ack);
    		FD_SET(sockfd,&read_ack);
    		select_result = select(sockfd+1,&read_ack,NULL,NULL,&timeout);
    		// Select set.
    		
    		if(select_result <0)
    		{
    			printf("Il y a une erreur sur le socket!");
				free(ack);
				exit(1);
    		}
    		else if(select_result == 0){ // Personne n'essaye de nous envoyer des donnees (ack).
    		
    			readFile(fp,tab_struct, TYPE, str, i*32); // On recupere str donnee dans notre tableau.
    			
				for(j= c%32; j<str; j++){ // Boucle du dernier ack recu à str.
				
					sleep(((float) delay)/((float) 1000)); // simulation du temps de parcours du "fil".
					//printf("numero de sequence envoye %d\n",tab_struct[j]->seqnum);
					if (random()%1000 < sber) // On coromp une partie des packets.
					{
   						tab_struct[j]->data[0] ^= 0xff;
					}
					
    				if (random()%100 < splr) // On envoie pas une partie des packets.
    				{}
					else if (sendto(sockfd, tab_struct[j], sizeof(struct packet), 0,p->ai_addr, p->ai_addrlen) == -1)
					{
						perror("sendto");
						free(tab_struct[j]);
						free(ack);
						exit(1);
					}
					free(tab_struct[j]); // On libere la memoire apres avoir envoye le packet.
				}
			}
				
			else if(select_result > 0 && FD_ISSET(sockfd,&read_ack)) // Un ack est en attente.
			{
				if (recvfrom(sockfd, ack, sizeof(struct packet) , 0,(struct sockaddr *)&their_addr, &addr_len) == -1)
				{	
					perror("recvfrom");
					free(ack);
					exit(1);
				}	
				
				else if((int)crc32(0L, (void*)ack, 516)==ack->CRC && (ack->type ==2)) // CRC OK & Type OK.
				{
					c = ack->seqnum; 
				}
			}
			
			if(c%32==(ack->window))// On a tout reçu puisqu'on a reçu le dernier.
			{
				boolean=1;	// On sort de la boucle.
			} // sinon on relance jusqu'a recevoir les ack manquants.
		}
	}
	close(sockfd);
	free(ack);
	fclose(fp);
	return 0;
}
