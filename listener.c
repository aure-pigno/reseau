#include "writeFile.c"

#define nbrStruct 32
#define TYPE_ACK 2

/* Fonction listener
 * @author : Martin Bontemps & Aurelien Pignolet
 * @date : 28 Octobre 2014
 * 
 * Cette fonction prend en argument le nom d'un fichier, le port auquel se connecter, l'adresse IP auquelle se connecter
 * Elle permet de recevoir des donnees et de les reecrire.
 * @pre : si filename == NULL alors sortie sur stdout
 */
 

int listener(char* filename, char* port, char* hostname)
{
	FILE* fp = NULL;
	if(filename == NULL)
	{
		fp = stdout;
	}
	
	else if ((fp = fopen(filename, "a" )) == NULL) 
	{
          fprintf(stderr, "Cannot open %s.\n", filename);
          return(-1);
    }
    
       	
   	int buf[nbrStruct]; // Tableau qui permet de savoir sion a recu une donnee ou non.
   	
   	int z;
   	for(z=0;z<nbrStruct;z++){
   		buf[z]=0;
   	}// On initialise le tableau

	struct packet* recevoir; // pointer qui permettra de recevoir une structure.	
	struct packet* ack = (struct packet*)malloc(sizeof(struct packet));	
	ack->type = TYPE_ACK;
	// Init du packet acknowledgment que l'on renverra.
	
	struct packet* tab_struct[nbrStruct];
	// Pointeur vers un tableau de structure ou sera stocké les donnees a envoyer.
	
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // mettre a AF_UNSPEC pour IPv4 et AF_INET pour IPv6
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
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
	// Initialisation du socket.
	
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	addr_len = sizeof their_addr;
	// Ou on stocke l'adresse de l'envoyeur.
	
	int boolean = 0; // Faux tant qu'on a pas recu une window!=31.
	int count = 0; // Compteur sur les fenetres.
	int n;
	int i = 0; // Compteur sur les numeros de sequence attendu.
	
	while(boolean == 0){
		recevoir = (struct packet*)malloc(sizeof(struct packet));	
		if ((numbytes = recvfrom(sockfd, recevoir, sizeof(struct packet) , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
		{
			perror("recvfrom");
			return -1;
		}
		// On recupere les donnees.
		//printf("numero de sequence recu %d numero de sequence attendu %d\n",recevoir->seqnum,i);
		if((int)crc32(0L, (void*)recevoir, 516)==recevoir->CRC && recevoir->type == 1){ // CRC OK & Type OK
			
			if ( (n = recevoir->seqnum ) > i){ // dans ce cas, on recoit un numero de sequence superieur au numero attendu
				
				if(buf[n%32] == 1){
				
				}// Cette structure est deja dans le buffer, on ne fait rien.
				
				else{ 
					tab_struct[n%32] = recevoir;
					ack->seqnum = i;
					ack->window = recevoir->window;
					ack->CRC = (int)crc32(0L, (void*)ack, 516);
					
					if(sendto(sockfd, ack, sizeof(struct packet), 0,(struct sockaddr *)&their_addr, addr_len)==-1)
					{
						perror("sendto");
						free(ack);
						free(recevoir);
						exit(1);
					}// On envoie l'ack.
					
					buf[n%32] = 1; // On indique qu'il est bien enregistre dans le buffer.
					
				}// On ajoute cette structure au buffer et on envoie le dernier ack valide du buffer.
			}
			else if(n==i) // On a reçu le bon numero de sequence.
			{	
				int k = n%32;
				tab_struct[k] = recevoir;
				buf[k] = 1; // On indique qu'il est bien enregistre dans le buffer.

				while(buf[k]==1 && k<32){k++;} // On met k au premier ack non recu.
				
				i = k+count*32; // i vaut la valeur suivante se seqnum que l'on droit avoir.
				if(i==256)
				{
					i=0;
					count=-1; 
				}
				ack->seqnum = i-1;
				ack->window = recevoir->window;
				ack->CRC = (int)crc32(0L, (void*)ack, 516);
				if(sendto(sockfd, ack, sizeof(struct packet), 0,(struct sockaddr *)&their_addr, addr_len) == -1){
					perror("sendto");
					free(ack);
					free(recevoir);
					exit(1);
				}
				// Envoie de l'ack.
				
				if(((ack->seqnum)%32) == recevoir->window){ // On est a la fin de la fenetre.
					
					boolean = (recevoir->window != 31); // sinon on était a la dernière fenetre.
					writeFile(fp,tab_struct, (recevoir->window+1));

					for(z=0;z<32;z++){
   						buf[z]=0;
   					}// On reinitialise le tableau de check.
					count++; // nbr de window totale
					//sleep(0.001); // Dans le cas ou la derniere structure est corrompue.
				}
			}

		}
	}	
	fclose(fp);
	
	printf("Le fichier d'arrivee contient %d caracteres\n",(int)file_size(filename));
	
	free(ack);
	close(sockfd);

	return 0;
}
