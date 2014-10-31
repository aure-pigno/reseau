#include "struct.h"

int writeFile(FILE *fp, struct packet **tab, int nStruct);
int readFile(FILE* fp, struct packet **tab, int type, int nbrStruct, int start);
long file_size(char *filename);

/* Fonction writeFile qui permet de lire un fichier.
 * @author : Martin Bontemps & Aurelien Pignolet
 * @date : 28 Octobre 2014
 *
 * La fonction prend en argument le nom du fichier dans lequel ecrire, un pointeur vers le tableau de structure
 * source et le nombre de structure a mettre dans le fichier.
 * La fonction va remplir le fichier avec les données contenues dans le tableau. Elle va liberer les zones de 
 * memoires allouees aus elements du tableau.
 */

int writeFile(FILE* fp, struct packet **tab, int nStruct)
{
    int i;
    for(i=0;fp !=NULL && i<nStruct;i++)
    {
    	fwrite( (tab[i]->data) , sizeof(uint8_t) , (tab[i]->length) , fp);
    	free(tab[i]);   		
    }
    
    return 0;
}

/* Fonction readFile qui permet de lire un fichier.
 * @author : Martin Bontemps & Aurelien Pignolet
 * @date : 28 Octobre 2014
 *
 * La fonction prend en argument le nom du fichier à lire, un pointeur vers un tableau de structure à remplir,
 * le type des structures à remplir et le nombre de structure a remplir.
 * La fonction va remplir la tableau 'tab' avec les données contenues dans le fichier. Elle va calculer le crc
 * et le numero de séquence (allant de 0 a 255).
 */

int readFile(FILE* fp, struct packet **tab, int type, int nbrStruct, int start)
{
    int i;
    fseek(fp,start*512,SEEK_SET); // On se met au bon endroit dans le fichier.
    for(i=0;i<nbrStruct;i++)
    {
  	  	struct packet *rep = (struct packet*) malloc(sizeof(struct packet));
		if(rep == NULL) {
			printf("Erreur lors de l'allocation de la mémoire\n");
			fclose(fp);
			return -1;
		}
    	rep->type = type;
    	rep->window = nbrStruct-1;
    	rep->seqnum = ((start+i)-(256*((start+i)/256)));
    	rep->length = TAILLE;
    	
    	int k = fread(rep->data, sizeof(uint8_t), TAILLE,fp);
		rep->length = k;
		
		rep->CRC = crc32(0L, (void*)rep, 516);
		
    	tab[i] = rep;
   	}
    
    return 0;

}

// Fonction qui récupère la taille du fichier passé en argument. Attention a ne pas avoir le fichier ouvert lors de l'execution.
long file_size(char *filename)
{
   struct stat s;
 
   if  (stat(filename,&s) != 0) {
      printf("error!\n" );
      return 0;
   }
 
   return (s.st_size);
} 
