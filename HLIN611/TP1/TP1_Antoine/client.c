#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

// J'ai volontairement modifié le type d'entré de char* vers void* 
// afin de pouvoir envoyer l'adresse d'un int (pour la taille du message à recevoir)
// sans générer de warning à la compilation
// J'ai également changé la valeur de retour, dans le cas ou l'envoie s'est bien passé,
// pour le nombre d'octets effectivement reçu au lieu de 1
int sendTCP(int addr, void* msg,int taille){
	int nbEnvoyer = 0; // Nombre d'octets envoyés au total
	int nbsend = 0; // Nombre d'octets envoyés à chaque tour de boucle
	while (nbEnvoyer < taille){
		// Si le client n'a pas renvoyé le bon nombre d'octets, il boucle
		nbsend = send(addr,msg+nbEnvoyer,taille-nbEnvoyer,0);
		switch(nbsend){
		
			case -1 : 
				perror("Client : erreur lors du send :"); 
				return -1;
			
			case 0 :  
				printf("Client : serveur déconnecté \n"); 
				return 0;
			
			default :
				nbEnvoyer = nbEnvoyer + nbsend;
				break;
		}
	}
	return nbEnvoyer;
}

int main(int argc, char *argv[]) {

	if (argc != 4){
		printf("Utilisation : %s ip_serveur port_serveur nombre_d_appels\n", argv[0]);
    	exit(0);
	}
	 
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
	
	printf("Client : demande de connexion réussie \n");
	
	printf("Saisir un message à envoyer (moins de 1500 caractères) \n");
	char m[1501]; 
	fgets(m, sizeof(m), stdin); 		      
	m[strlen(m)-1]  = '\0';                                 
	
	int res = atoi(argv[3])*(strlen(m));
	int snd = sendTCP(ds,&res,sizeof(res));
		
	if(snd < 0){
	 	printf("Client : erreur lors du send du nombre d'octet à lire !");
	 	close(ds);
	}
	if(snd == 0){
		close(ds);
	}
	int nombre_D_Envoi = 0;
	int snd1 = 0;
	int totalSend = 0;
	while(nombre_D_Envoi < atoi(argv[3])){
		snd1 = sendTCP(ds,m,strlen(m)); 
		
		if(snd1 < 0){
	 		printf("Client : erreur lors du send du %d envoi !\n",nombre_D_Envoi);
	 		close(ds);
	 		break;
		}else if(snd1 == 0){
			close(ds);
			break;
		}else{
			totalSend += snd1;
		}
		nombre_D_Envoi ++;
	}
	
	
	
	printf("Client : j'ai déposé %d octets au total en %d appel à send avec un message de %lu octets\n",totalSend,nombre_D_Envoi,strlen(m));
	

  	close (ds);
  	printf("Client : je termine\n");
}
