#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>


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


int main(int argc, char *argv[])
{
	 
  
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
	
	long int messagesRecus[500];   
	unsigned int nbTotalOctetsRecus = 0;
	unsigned int nbAppelRecv = 0;
	
	fd_set set;
	FD_ZERO(&set);
	FD_SET(ds, &set);
	fd_set setPret;
	int pGrandSocketObserve = ds;
	while(1){
		setPret = set;
		int nbSocketPret = select(pGrandSocketObserve+1, &setPret,NULL,NULL,NULL);
		printf("setPret contient les %d socket que je souhaite observer et qui ont quelque chose à lire \n",nbSocketPret);
		
		for(int df= 2 ; df <= pGrandSocketObserve ; df++){
			if (FD_ISSET(df, &setPret)) {
				if (df == ds) {
	
					int dsCv= accept(ds, (struct sockaddr *)&adCv,&lgCv); ; 
					FD_SET(dsCv, &set) ;
					if (dsCv < 0){ 
	    				perror ( "Serveur : problème lors de l'exécution de la fonction accept :");
	    				close(ds);
					}
					if (pGrandSocketObserve < dsCv){
						pGrandSocketObserve = dsCv;
					}	
				
					printf("Serveur : le client %s:%d de numSocket %d est connecté  \n", inet_ntoa(adCv.sin_addr), ntohs(adCv.sin_port), dsCv);
				} else {
					int rcv = recvTCP (df, (char*)messagesRecus, atoi(argv[2]), &nbTotalOctetsRecus, &nbAppelRecv);  
					if (rcv > 0) {
						printf("Serveur : nouvelle réception de %d octet du client numSocket %d \n",rcv, df);
						printf("Serveur : j'ai reçu au total %d octets avec %d appels à recv \n", nbTotalOctetsRecus, nbAppelRecv);
					}else {
						FD_CLR(df, &set);
						close(df);
						printf("Serveur : le client de numSocket %d est déconnecté  \n", df);
						while(!FD_ISSET(pGrandSocketObserve,&set)){
							pGrandSocketObserve --;
						}
					}
				}
			}
		}
	}
		
  
  	close(ds);
  	printf("Serveur : je termine\n");
  	return 0;
}








