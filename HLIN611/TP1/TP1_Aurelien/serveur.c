#include "fonc.h"
#define __SERVEUR__

int main(int argc, char **argv){
    // je passe en paramètre un numero de port pour la socket
    // d'écoute. Je teste donc le passage de paramètres

    if (argc<2){
        printf("utilisation: %s numero_port\n", argv[0]);
        exit(1);
    }

    /* Etape 1 : créer une socket (la socket qui permettra de recevoir
       les demandes de connexion.*/

    int ds = socket(PF_INET, SOCK_STREAM, 0);
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
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons((short) atoi(argv[1])); // Host to network (short)

    if(bind(ds, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
        perror("Serveur : erreur bind");
        close(ds); // je libère les ressources avant de terminer.
        exit(1); // je choisis de quitter le programme : la suite dépend de la réussite du nommage.
    }

    // je continue à alimenter le programme avec des traces d'exécution
    printf("Serveur: nommage : ok\n");


    // Je peux tester l'exécution de cette étape avant de passer à la suite.


    /* Etape 3 : mise en ecoute de la socket nommée. Cette opération
       dédie la socket à la réception des demandes de connexion. Pour
       cet exercice, limiter la file des demandes de connexions à 5. */


    int ecoute = listen(ds, 5);
    if (ecoute < 0){
        printf("Serveur : je suis sourd(e)\n");
        close (ds);
        exit (1);
    }

    // je continue à alimenter le programme avec des traces d'exécution
    printf("Serveur: mise en écoute : ok\n");


    // Je peux tester l'exécution de cette étape avant de passer à la suite.


    /* etape 4 : attendre et traiter une demande de connexion d'un
       client. Rappel : lorsque le serveur accepte une demande de
       connexion, il crée une nouvelle socket. Cette dernière est
       connectée à celle du client et elle est à utiliser pour
       communiquer avec lui.*/

    printf("Serveur : j'attends la demande d'un client (accept) \n");

    struct sockaddr_in adCv ; // pour obtenir l'adresse du client accepté.
    socklen_t lgCv = sizeof (struct sockaddr_in);

    int nbOctEchanges=0, nbRecv=0, clientDisconected=1, dsCv;
    while(1){
        if(clientDisconected){
            dsCv = accept(ds, (struct sockaddr *) &adCv, &lgCv);
            if (dsCv < 0){ // je pense toujours à traiter les valeurs de retour.
                perror ( "Serveur, probleme accept :");
                close(ds);
                exit (1);
            }
            clientDisconected=0;
            printf("Serveur: le client %s:%d est connecté  \n", inet_ntoa(adCv.sin_addr), ntohs(adCv.sin_port));
        }

        int taille=0;
        int rcv = recvTCP(dsCv, (char*)&taille, sizeof(int));

        if(rcv <= 0){
            close(dsCv);
            break;
        }

        nbOctEchanges+=sizeof(int);
        nbRecv++;

        printf("Taille du message: %d\n",taille);
        /* Etape 5 : réception d'un message de type chaîne de caractères */

        char *buffer = malloc(sizeof(char)*(taille +1));
        rcv = recvTCP(dsCv, buffer, taille);
        if(rcv <= 0){
            close(dsCv);
            break;
        }
        nbRecv++;
        nbOctEchanges+=taille;
        buffer[taille+1]='\0';  /* On rajoute la fin de chaîne*/
        printf("\nServeur : j'ai recu: %d octets au total\n", nbOctEchanges);
        printf("Serveur : j'ai appelé %d fois recvTCP\n", nbRecv);
        printf("Serveur : contenu du message : \n\t%s \n\n", buffer);

        if(strcmp(buffer,"quitter")==0){
            printf("\nServeur : Le client s'est déconnecté manuellement\n");
            free(buffer);
            close(dsCv);
            clientDisconected=1;
        }
        else{
            free(buffer);
            int snd = sendTCP(dsCv, (char*)&taille, sizeof(taille));
            if(snd <= 0){
                close(dsCv);
                clientDisconected=1;
            }
            else{
                continue;
            }
        }
        /* Etape 6 : fermeture de la socket du client */
        printf("Serveur : fin du dialogue avec le client\n");
    }

    close (ds);
    printf("Serveur : je termine\n");
}
