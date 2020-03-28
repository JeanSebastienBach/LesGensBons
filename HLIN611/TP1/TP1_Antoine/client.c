#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>


int sendTCP(int addr, char* msg,int taille){
	int nbEnvoyer = 0; // Nombre d'octets envoyés au total
	int nbsend = 0; // Nombre d'octets envoyés à chaque tour de boucle
	while (nbEnvoyer < taille){
		// Si le client n'a pas renvoyé le bon nombre d'octets, il boucle
		nbsend = send(addr,msg+nbEnvoyer,taille-nbEnvoyer,0);
		
		switch(nbsend){
		
			case -1 : 
				return -1;
			
			case 0 :  
				return 0;
			
			default :
				nbEnvoyer = nbEnvoyer + nbsend;
				break;
		}
	}
	return 1;
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
	
	int nombre_D_Envoi = 0;
	int snd = 0;
	int snd1 = 0;
	int taille = strlen(m);
	while(nombre_D_Envoi < atoi(argv[3])){
		snd = sendTCP(ds,&taille,sizeof(taille));
		if(snd < 0){
	 		perror("Client : erreur lors du send de la taille! :");
	 		close(ds);
	 		break;
		}else if(snd == 0){
			printf("Client : serveur déconnecté \n"); 
			close(ds);
			break;
		}
		snd1 = sendTCP(ds,m,taille); 
		if(snd1 < 0){
	 		perror("Client : erreur lors du send ! :");
	 		close(ds);
	 		break;
		}else if(snd1 == 0){
			printf("Client : serveur déconnecté \n"); 
			close(ds);
			break;
		}
		nombre_D_Envoi ++;
	}
	
	
	
	printf("Client : j'ai déposé %lu octets au total en %d appel à send avec un message de %lu octets\n",strlen(m)*atoi(argv[3]),nombre_D_Envoi,strlen(m));
	

  	close (ds);
  	printf("Client : je termine\n");
}
