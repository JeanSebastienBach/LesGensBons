#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

// Rôle du serveur : accepter la demande de connexion d'un client,
// recevoir une chaîne de caractères, afficher cette chaîne et
// renvoyer au client le nombre d'octets reçus par le serveur.

int main(int argc, char *argv[])
{
  // je passe en paramètre un numero de port pour la socket
  // d'écoute. Je teste donc le passage de paramètres
  
  if (argc<2){
    printf("utilisation: %s numero_port\n", argv[0]);
    exit(1);
  }
  
  /* Etape 1 : créer une socket (la socket qui permettra de recevoir
     les demandes de connexion.*/
 
  int ds = socket(PF_INET,SOCK_STREAM,0);
  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs possibles.*/
  if (ds == -1){
    perror("Serveur : probleme creation socket");
    exit(1); // je choisis ici d'arrêter le programme car le reste
	     // dépendent de la réussite de la création de la socket.
  }
  

  // J'alimente le programme avec traces d'exécution pour observer ce
  // qui se produit à l'exécution et mieux localiser les erreurs. 
  printf("Serveur: creation de la socket : ok\n");


  // Je peux déjà tester l'exécution de cette étape avant de passer à la suite.
  
  /* Etape 2 : nommer la socket. Elle aura une ou des IP de la machine
     sur laquelle le programme sera exécuté et le numéro de port passé
     en paramètre*/

  
  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY; // Je me connect à l'importe quel adresse sur laquel je peut accéder avec mon programme
  serverAddr.sin_port = htons((short)atoi(argv[1])); //short car normalement c'est un u_short qui est envoyé
  
  if(bind(ds,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0){
    perror("Serveur : erreur bind");
    close(ds); // je libère les ressources avant de terminer.
    exit(1); // je choisis de quitter le programme : la suite dépend de la réussite du nommage.
  }

  // je continue à alimenter le programme avec traces d'exécution
  printf("Serveur: nommage : ok\n");


  // Je peux tester l'exécution de cette étape avant de passer à la suite.


  /* Etape 3 : mise en ecoute de la socket nommée. Cette opération
     dédie la socket à la réception des demandes de connexion. Pour
     cet exercice, limiter la file des demandes de connexions à 5. */
  
  int ecoute = listen(ds,5); //(int socket , backlog)
  if (ecoute < 0){ //réseau deconnecté par exemple 
    printf("Serveur : je suis sourd(e)\n");
    close (ds);
    exit (1);
  } 
 
  // je continue à alimenter le programme avec traces d'exécution
  printf("Serveur: mise en écoute : ok\n");


  // Je peux tester l'exécution de cette étape avant de passer à la suite.

 
  /* etape 4 : attendre et traiter une demande de connexion d'un
     client. Rappel : lorsque le serveur accepte une demande de
     connexion, il crée une nouvelle socket. Cette dernière est
     connectée à celle du client et elle est à utiliser pour
     communiquer avec lui.*/

  // avant un appel à une fonction bloquante, il est intéressant
  // d'afficher un message le signalant. Vous saurez ainsi à quelle
  // étape de l'exécution le blocage se produit.
  printf("Serveur : j'attends la demande d'un client (accept) \n"); 
  
  struct sockaddr_in adCv ; // pour obtenir l'adresse du client accepté.
  socklen_t lgCv = sizeof (struct sockaddr_in);


//socket serveur pour attendre les client (bin, listen , accept)
  //socket client ( du coté client) qui sera utilisé pour (connect)
  //le accept coté serveru me crée un new socket coté serveur pour dialoguer avec ce client : dsCv (socket dédier a la connection avec ce client , circuis virtuel crée par le accept)
 // car on cree un circuit virtuel en mode connecté (dscv , socket client connu pas sa sockaddr_in)

  int dsCv = accept(ds,(struct sockaddr *)&adCv,&lgCv);
  if (dsCv < 0){ // je pense toujours à traiter les valeurs de retour.
    perror ( "Serveur, probleme accept :");
    close(ds);
    exit (1);
  }
  /* je peux afficher l'adresse de la socket du client accepté :
     adresse IP et numéro de port de la structure adCv. Attention à
     faire les conversions du format réseau vers le format
     hôte. Utiliser la fonction inet_ntoa(..) pour l'IP.*/
  printf("Serveur: le client %s:%d est connecté  \n", inet_ntoa(adCv.sin_addr),  ntohs(adCv.sin_port));

  // Je peux tester l'exécution de cette étape avant de passer à la suite.
 
  /* Etape 5 : réception d'un message de type chaîne de caractères */
 
  char buffer[500];
  /* attendre un message dont la taille maximale est 500 octets. Pour
     cet exercice, il est demandé de ne faire qu'un seul appel à recv
     pour recevoir un message. */

  //man de reveive (socket , *buffer , size_t legth , int flags)
  int rcv = recv (dsCv,buffer,sizeof(buffer), 0) ;  //car le buffer est un tableau static sinon ce serait le nombre de pointeurs  donc ça fais 500
  //rcv renvoie le vrai nombre d'octet reçus 

  /* Traiter TOUTES les valeurs de retour (voir le cours ou la documentation). */

  if(rcv<0) {//très commun
    perror("Serveur : erreur à la reception :") ; 
    close (dsCv);
    //se poser la question de continuer ou pas l'execuion
  }


    //close avant de de ce deconnecter

  if (rcv==0)
  {
    perror("Serveur client déconnecté : \n");
    close(dsCv);
    //se poser la question de continuer ou pas l'execuion
  }


  /* Afficher le nombre d'octets EFFECTIVEMENT reçus. : /!\ Faire la
     différence entre le nombre d'octets qu'on demande à extraire
     depuis le buffer de réception d'une socket et le nombre d'octets
     qu'on a effectivement réussi à extraire.*/
  
  printf("Serveur : j'ai recu %d octets \n", rcv); // on peut en rece voir 50 sur les 150
  printf("Serveur : contenu du message : %s \n", buffer);

  // Je peux tester l'exécution de cette étape avant de passer à la suite.
  
  /* Etape 6 : répondre au client en lui envoyant le nombre d'octets
     effectivement reçus à l'étape 5. Pour cet exercice, faire un seul
     appel de la fonction send.*/ 
  int snd = send(dsCv,&rcv,sizeof(rcv),0); //on envoie les octect correspondant à mon octet le nombre d'octer qui représente le codage de 150 par exemple (pas un pour 1 , 1 pour 5 et 1 pour 0)
  
  /* Traiter TOUTES les valeurs de retour (voir le cours ou la documentation). */

  // -1  si ça ce passe pas bien 
  // ou un nbr d'octet réellement envoyé 

  if (snd < 0)
  {
    perror("Serveur : erreur send\n");
    close(dsCv);
    //se poser la question de continuer ou pas l'execuion
  }

  if (snd == 0) 
  {
    printf("Serveur : client déconnecté\n" );
    close(dsCv);
    //se poser la question de continuer ou pas l'execuion
  }

    
  /* Etape 7 : fermeture de la socket du client */ 
  printf("Serveur : fin du dialogue avec le client\n");
  close (dsCv);
  
  // Etape 8 : pour cet exercice, je ne traite qu'un client, donc, je termine proprement.
  close (ds);
  printf("Serveur : je termine\n");
}








