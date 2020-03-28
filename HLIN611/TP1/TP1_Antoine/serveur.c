#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

int recevoirTCP(int addr,char* s,int taille){
	int nbOctetRecu = 0; // Nombre d'octets reçus au total
	int nbrecv = 0; // Nombre d'octets reçus à chaque tour de boucle
	while(nbOctetRecu < taille){
    // Si le serveur n'a pas reçu le nombre d'octets attendu, il boucle
		nbrecv = recv(addr,s+nbOctetRecu,taille-nbOctetRecu,0);
		switch(nbrecv){
			case -1 : 
				return -1;
			
			case 0 :  
				return 0;
			
			default :
				nbOctetRecu = nbOctetRecu + nbrecv;
				break;
		}
	}
	return 1;
}
int main(int argc, char *argv[])
{
if (argc<2){
    printf("Utilisation : %s numero_port\n", argv[0]);
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

printf("Serveur : j'attends la demande d'un client (accept) \n"); 
while(1){
  
	struct sockaddr_in adCv ; 
	socklen_t lgCv = sizeof (struct sockaddr_in);

	int dsCv = accept(ds, (struct sockaddr *)&adCv,&lgCv);
	if (dsCv < 0){ 
    	perror ( "Serveur : problème lors de l'exécution de la fonction accept :");
    	close(ds);
    	exit (1);
	}
  
  	printf("Serveur : le client %s:%d est connecté  \n", inet_ntoa(adCv.sin_addr), ntohs(adCv.sin_port));
  	int nbOctetTotal = 0;
  	int nbEchange = 0;
  	while (1){

    	int taille = 0;
    	int err1 = recevoirTCP(dsCv,(char*)&taille,sizeof(taille));
    	if(err1 == -1) {
    		perror ( "Serveur : problème lors de la réception :");
    		close (dsCv) ;
    	    break; 
    	}
    	if(err1 == 0) {
    		printf( "Serveur : client déconnecté ! \n");
    		close (dsCv) ;
    	    break; 
    	}
    	printf("Serveur : je dois recevoir %d octet(s) : ",taille);
    	char* buffer = malloc(sizeof(char)*(taille +1));
    	int err = recevoirTCP(dsCv, buffer,taille);
    	if(err1 == -1) {
    		perror ( "Serveur : problème lors de la réception :");
    		close (dsCv) ;
    	    break; 
    	}
    	if(err1 == 0) {
    		printf( "Serveur : client déconnecté ! \n");
    		close (dsCv) ;
   	 		break; 
   		}

  		
  		printf("Serveur : j'ai reçu %d octet(s) \n", err*taille);
  		printf("Serveur : contenu du message : <%s> \n", buffer);
  		free(buffer);
  		nbEchange++;
  		nbOctetTotal = nbOctetTotal + err*taille;
  	}
  	
  	printf("Serveur : fin du dialogue avec le client\n");
  	printf("Serveur : le client à communiqué %d fois pour un total d'octets envoyé égale à %d\n",nbEchange,nbOctetTotal);
  	close (dsCv);
}
  
close (ds);
printf("Serveur : je termine\n");
}
