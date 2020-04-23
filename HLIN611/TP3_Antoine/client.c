#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <sys/stat.h>



#define MAX_BUFFER_SIZE 16000  // taille du buffer temporaire pour la lecture de fichier. Vous pouvez définir une autre valeur.


int sendTCP(int socket, const char * buffer, size_t length, unsigned int *nbBytesSent, unsigned int * nbCallSend){
	int sent = 0;
	int nbOctetEnvoyer = 0;
  	while (nbOctetEnvoyer < length){
    	
    	sent = send(socket,buffer+nbOctetEnvoyer,length-nbOctetEnvoyer,0);
    	
    	if (sent <= 0) {
    	  return sent;
    	}
		nbOctetEnvoyer += sent;
    	(*nbBytesSent)+=sent;
    	(*nbCallSend)++;
  	}
  	return 1;
  
}


int main(int argc, char *argv[]) {

	if (argc != 4){
    	printf("utilisation : client ip_serveur port_serveur nom_fichier\n");
    	exit(0);
  	}
  	
  	// je construis le nom complet (chemin) du fichier que je dois lire
  	char* filepath = malloc(strlen(argv[3]) + 12); // ./emission/+nom fichier +\0
  	filepath[0] = '\0';
  	strcat(filepath, "./emission/");
  	strcat(filepath, argv[3]);
	
	
	// je récupère la taille du fichier. Pourquoi ?
	struct stat attributes;
  	if(stat(filepath, &attributes) == -1){
    	perror("Client : erreur stat");
    	free(filepath);
    	exit(1);
  	}

  	int file_size = attributes.st_size;  // c'est bon, j'ai obtenu la taille du fichier. Refléchir ce que je dois en fair .


  	// pour envoyer le contenu, je dois lire le fichier :
    FILE* file = fopen(filepath, "rb"); // ouverture en lecture
  	if(file == NULL){
    	perror("Client : erreur ouverture fichier \n");
    	free(filepath);
    	exit(1);   
  	}
  	
  	printf("Client: je suis prêt à envoyer %s\n", filepath);
  	
  	free(filepath); // je n'ai plus besoin de ce tableau dans la suite.
  	

 	int ds = socket(PF_INET,SOCK_STREAM,0);
	
	if (ds == -1){
	    printf("Client : problème création socket\n");
	    exit(1); 
	}
	printf("Client : création de la socket : ok\n");
	  
	struct sockaddr_in adrServ;
	adrServ.sin_family = AF_INET;
	adrServ.sin_addr.s_addr = inet_addr(argv[1]) ;
	adrServ.sin_port = htons((short)atoi(argv[2]));
	    
	int lgAdr = sizeof(struct sockaddr_in);
	    
	int conn = connect(ds,(struct sockaddr *)&adrServ,lgAdr);
	  
	if (conn < 0){
	    perror ("Client : problème au connect :");
	    close (ds);
	    exit (1); 
	}
	

  
  	printf("Client : demande de connexion reussie \n");

	

    // envoyer le nom de fichier (réfléchir aux données à envoyer pour que le nom soit corectement reçu)

  	int name_size = strlen(argv[3]) + 1;
	unsigned int nbTotalOctetsEnvoyes = 0;
  	unsigned int nbAppelSend = 0;
	int snd = sendTCP(ds,(char*)&name_size,sizeof(name_size),&nbTotalOctetsEnvoyes,&nbAppelSend);
	if(snd < 0){
		perror("Client : erreur lors du send :");
 		close(ds);
		exit(1);
	}else if(snd == 0){
		printf("Client : serveur déconnecté \n"); 
		close(ds);
		exit(1);
	}
   	printf("Client : J'ai envoyé la taille %d du nom fichier \n",name_size); 
   	snd = sendTCP(ds,argv[3],name_size,&nbTotalOctetsEnvoyes,&nbAppelSend);
	if(snd < 0){
		perror("Client : erreur lors du send :");
 		close(ds);
		exit(1);
	}else if(snd == 0){
		printf("Client : serveur déconnecté \n"); 
		close(ds);
		exit(1);
	}
   	
	printf("Client : J'ai envoyé le nom %s du fichier \n",argv[3]); 
	snd = sendTCP(ds,(char*)&file_size,sizeof(file_size),&nbTotalOctetsEnvoyes,&nbAppelSend);
	if(snd < 0){
		perror("Client : erreur lors de l'envoie la taille du fichier :");
 		close(ds);
		exit(1);
	}else if(snd == 0){
		printf("Client : serveur déconnecté \n"); 
		close(ds);
		exit(1);
	}
	printf("Client : J'ai envoyé la taille %d du fichier \n",file_size); 
  	// je fait une lecture par bloc.
  	// je continue à lire tant que je n'ai pas lus le fichier en entier.
  	int numBloc = 0;
  	int nbRead = 0;//
  	while(nbRead < file_size){
    	numBloc++;
    	char buffer[MAX_BUFFER_SIZE];
    	size_t read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, file);
    	// je viens de tenter de lire un bloc d'au maximum MAX_BUFFER_SIZE octets
    	if(read == 0){
    		if(ferror(file) != 0){
				perror("Client : erreur lors de la lecture du fichier \n");
				fclose(file);
				exit(1);
      		} // fin du fichier
      		break; // plus rien à lire
    	}
    	// ici j'ai un bloc de read octets lus et stoqués dans buffer. Que faire de ce bloc d'octets ?
  	
   		snd = sendTCP(ds,buffer,read,&nbTotalOctetsEnvoyes,&nbAppelSend);
		if(snd < 0){
			perror("Client : erreur lors de l'envoie d'un bloc :");
 			close(ds);
			break;
		}else if(snd == 0){
			printf("Client : serveur déconnecté \n"); 
			close(ds);
			break;
		}
		
    	printf("Client : J'ai envoyé le bloc %d donc les octets de %d à %lu du fichier : ",numBloc,nbRead,nbRead+read); 
  	  	nbRead += read;
  	}
	printf("Client : J'ai envoyé le fichier  entier en %d bloc(s) \n",numBloc); 
  	// fermeture du fichier
  	int resc = fclose(file);
  	if(resc){
  		perror("Client : Probleme de fermeture du fichier \n");
  	}
  	printf("Client : j'ai envoye au total : %d octets,  envoyes en %d appels a send \n", nbTotalOctetsEnvoyes,nbAppelSend);  
  	close (ds);
  	printf("Client : je termine\n");
  	return 0;
}
