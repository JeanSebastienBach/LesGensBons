#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>


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
    printf("utilisation : %s ip_serveur port_serveur N_nombre_entiers_e_envoyer\n", argv[0]);
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
	

  
  printf("Client : demande de connexion reussie \n");



  	long int message;
  	unsigned int nbTotalOctetsEnvoyes = 0;
  	unsigned int nbAppelSend = 0;
  	int snd;
	
	for(int i = 1; i <= atoi(argv[3]); i++){
	    message = i; 
		    
    	snd = sendTCP(ds,&message,sizeof(message),&nbTotalOctetsEnvoyes,&nbAppelSend);
		if(snd < 0){
			perror("Client : erreur lors du send :");
 			close(ds);
		 	break;
		}else if(snd == 0){
			printf("Client : serveur déconnecté \n"); 
			close(ds);
			break;
		}
	}
   	printf("Client : j'ai envoyé au total %d octets avec %d appels à send \n",nbTotalOctetsEnvoyes,  nbAppelSend) ;

  	
  
	close(ds);
  	printf("Client : je termine\n");
  	return 0;
}
