#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>


#define MAX_BUFFER_SIZE 16000 // taille du buffer qui me permet de récupérer le contenu du fichier à recevoir bloc par bloc. Vous pouvez changer cette valeur.

int recvTCP(int socket, char *buffer, size_t length, unsigned int *nbBytesReceved, unsigned int * nbCallRecv){
	int nbOctetRecu = 0; 
	int received = 0;
    while (nbOctetRecu < length){
  	
    	received = recv(socket,buffer+nbOctetRecu,length-nbOctetRecu,0);

      	if(received <= 0){
        	return received;
      	}
      	nbOctetRecu += received;
      	(*nbBytesReceved)+=received;
      	(*nbCallRecv)++;
    }
    return 1;
}

int main(int argc, char *argv[]){
  
  
	if (argc<2){
    	printf("utilisation: %s numero_port\n", argv[0]);
    	exit(1);
  	}
  
    
	int ds = socket(PF_INET, SOCK_STREAM,0);
	
	if (ds == -1){
	    perror("Serveur: problème création socket");
	    exit(1); 
	}
	  
	printf("Serveur : création de la socket : ok\n");
	
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons((short)atoi(argv[1]));
	  
	if(bind(ds,(struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
	    perror("Serveur : erreur bind");
	    close(ds); 
	    exit(1); 
	}
	
	printf("Serveur : nommage : ok\n");
	
	int ecoute = listen(ds, 5);
	if (ecoute < 0){
    	printf("Serveur : je suis sourd(e)\n");
    	close (ds);
    	exit (1);
	} 

	printf("Serveur : mise en écoute : ok\n");
	
		
	
	struct sockaddr_in adCv ; 
	socklen_t lgCv = sizeof (struct sockaddr_in);
	unsigned int nbTotalOctetsRecus = 0;
	unsigned int nbAppelRecv = 0;
	int name_size = 0;
	
 	/* boucle pour le traitement itératif des clients */
 	while(1){

  		int dsCv= accept(ds, (struct sockaddr *)&adCv,&lgCv); ; 
    	// je doit recevoir des données me permettant d'obtenur le nom d'un fichier
    	if (dsCv < 0) {
    		perror("serveur : pb accept");
			close(ds);
			exit(1);
		}
    	name_size = 0;
    	nbTotalOctetsRecus = 0;
    	nbAppelRecv = 0;
    	int rcv = recvTCP (dsCv, (char*)&name_size, sizeof(int), &nbTotalOctetsRecus, &nbAppelRecv);  
		if (rcv <= 0) {
			close(dsCv);
			printf("Serveur : le client est déconnecté  \n");
			break;
		}
    	
    	char* file_name = malloc(name_size);
		
		rcv = recvTCP (dsCv, file_name, name_size, &nbTotalOctetsRecus, &nbAppelRecv);  
		if (rcv <= 0) {
			close(dsCv);
			printf("Serveur : le client est déconnecté  \n");
			break;
		}
		
    	printf("Serveur : je vais recevoir le fichier %s\n", file_name);
	
	    // je construis le chemin vers le fichier à créer.
	    char* filepath = malloc(name_size + 13); // ./reception/+nom fichier
	    filepath[0] = '\0';
	    strcat(filepath, "./reception/");
	    strcat(filepath, file_name);
	    free(file_name);
	
	  // j'ouvre le fichier dans lequel je vais écrire
	    FILE* file = fopen(filepath, "wb");
	    if(file == NULL){  // servez vous de cet exemple pour le traitement des erreurs. 
	    	perror("Serveur : erreur ouverture fichier: \n");
	      	free(filepath);
      		close (dsCv);
      		break; // passer au client suivant 
    	}
    	
    	free(filepath); // je n'en ia plus besoin.

    	//reception des données me permettant de recevoir correctement le contenu du fichier.
    	int file_size;
		
      	rcv = recvTCP (dsCv, (char*)&file_size, sizeof(int), &nbTotalOctetsRecus, &nbAppelRecv);  
		if (rcv <= 0) {
			close(dsCv);
			printf("Serveur : le client est déconnecté  \n");
			break;
		}
		printf("Serveur : je vais recevoir le fichier de taille %d \n", file_size);
		
    	int contentReceived = 0; // Compte le nombre d'octets du fichier reçu

    	// je reçois le contenu progressivement 
    	while(contentReceived < file_size){
      		
     		char buffer[MAX_BUFFER_SIZE];
     		int realSize = file_size - contentReceived;
     		if (MAX_BUFFER_SIZE < realSize){
     			realSize = MAX_BUFFER_SIZE;
     		}
     		printf("Serveur : je vais recevoir %d octet\n", realSize);
     		rcv = recvTCP (dsCv, buffer,realSize, &nbTotalOctetsRecus, &nbAppelRecv);  
			if (rcv <= 0) {
				close(dsCv);
				printf("Serveur : le client est déconnecté  \n");
				break;
			}
			printf("Serveur : j'ai reçu %d octet\n", realSize);

      		// si pas d'erreurs, j'ai reçu rcv octets. Je dois les écire dans le fichier.
      		size_t written = fwrite(buffer, sizeof(char), realSize, file);
      		if(written < realSize){
				perror("Serveur : Erreur a l'ecriture du fichier \n");
	  			break; // je sors de la boucle d'écrture/réception.
      		}

      		contentReceived += realSize;
    	}

    // fermeture du fichier à la fin de son écriture ou si erreur s'est produite.
    
    fclose(file);
    printf("Serveur : fin du dialogue avec le client, nombre total d'octets recus : %d,  recus en %d appels a recv \n", nbTotalOctetsRecus, nbAppelRecv);
    close (dsCv);
    //je passe au client suivant.
  }
  
  close (ds); // atteignable si on sort de la boucle infinie.
  printf("Serveur : je termine\n");
 }








