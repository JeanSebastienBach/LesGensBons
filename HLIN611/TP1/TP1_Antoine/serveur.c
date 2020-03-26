#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

// J'ai volontairement modifié le type d'entré de char* vers void* 
// afin de pouvoir envoyer l'adresse d'un int (pour la taille du message à recevoir)
// sans générer de warning à la compilation
// J'ai également changé la valeur de retour, dans le cas ou la reception c'est bien passé, 
// pour le nombre d'octet effectivement reçu au lieu de 1
int recevoirTCP(int addr,void* s,int taille){
	int nbOctetRecu = 0;
	int nbrecv = 0;
	while(nbOctetRecu < taille){
		nbrecv = recv(addr,s+nbOctetRecu,taille-nbOctetRecu,0);
		switch(nbrecv){
		
			case -1 : 
				perror("Erreur de réception serveur"); 
				return -1;
			
			case 0 :  
				printf("Client déconnecté lors de réception serveur\n"); 
				return 0;
			
			default :
				nbOctetRecu = nbOctetRecu + nbrecv;
				break;
		}
	}
	return nbOctetRecu;
}
int main(int argc, char *argv[])
{
if (argc<2){
    printf("utilisation: %s numero_port\n", argv[0]);
    exit(1);
}
  
int ds = socket(PF_INET, SOCK_STREAM,0);

if (ds == -1){
    perror("Serveur: probleme creation socket");
    exit(1); 
}
  
printf("Serveur: creation de la socket : ok\n");

struct sockaddr_in serverAddr;
serverAddr.sin_family = AF_INET;
serverAddr.sin_addr.s_addr = INADDR_ANY;
serverAddr.sin_port = htons((short)atoi(argv[1]));
  
if(bind(ds,(struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
    perror("Serveur: erreur bind");
    close(ds); 
    exit(1); 
}

printf("Serveur: nommage : ok\n");

int ecoute = listen(ds, 5);
if (ecoute < 0){
    printf("Serveur: je suis sourd(e)\n");
    close (ds);
    exit (1);
} 

printf("Serveur: mise en écoute : ok\n");

printf("Serveur: j'attends la demande d'un client (accept) \n"); 
while(1){
  
	struct sockaddr_in adCv ; 
	socklen_t lgCv = sizeof (struct sockaddr_in);

	int dsCv = accept(ds, (struct sockaddr *)&adCv,&lgCv);
	if (dsCv < 0){ 
    	perror ( "Serveur: probleme accept :");
    	close(ds);
    	exit (1);
	}
  
  	printf("Serveur: le client %s:%d est connecté  \n", inet_ntoa(adCv.sin_addr), ntohs(adCv.sin_port));

	char buf[5]; 
	printf("Veuillez saisir OK si vous etes prêt : ");
  	fgets(buf, sizeof(buf), stdin); 
 
    int taille = 0;
    int err1 = recevoirTCP(dsCv, &taille,sizeof(taille)) ;
    if(err1 <= 0) {
    	close (dsCv) ;
        break; 
    }
    printf("Serveur: je dois recevoir %d octet : ",taille);
    char* buffer = malloc(sizeof(char)*(taille +1));
    int err = recevoirTCP(dsCv, buffer,taille) ;
    if(err <= 0) {
        close (dsCv) ;
        break;
    }
    buffer[err]='\0'; 
  
  	printf("Serveur: j'ai recu %d octets \n", err);
  	printf("Serveur: contenu du message : <%s> \n", buffer);
  	
  	printf("Serveur: fin du dialogue avec le client\n");
  	close (dsCv);
}
  
close (ds);
printf("Serveur: je termine\n");
}








