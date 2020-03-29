#include "fonc.h"

int main(int argc, char**argv){


    /* je passe en paramètre l'adresse de la socket d'écoute du serveur
       (IP et numéro de port). Je teste donc le passage de parametres */

    if (argc != 3){
        printf("utilisation : %s ip_serveur port_serveur\n", argv[0]);
        exit(0);
    }

    /* Etape 1 : créer une socket */
    int ds = socket(PF_INET,SOCK_STREAM,0);

    /* /!\ : Il est indispensable de tester les valeurs de retour de
       toutes les fonctions et agir en fonction des valeurs possibles.*/
    if (ds == -1){
        printf("Client : pb creation socket\n");
        exit(1); // je choisis ici d'arrêter le programme car le reste
        // dépendent de la réussite de la création de la socket.
    }

    /* J'alimente le programme avec des traces d'exécution pour observer ce
       qui se produit à l'exécution et pour mieux localiser les erreurs. */
    printf("Client: creation de la socket : ok\n");

    // Je peux déjà tester l'exécution de cette étape avant de passer à la suite.

    /* Etape 2 : designer la socket du serveur : avoir une structure qui
       contient l'adresse de cette socket (IP + numéro de port. */
    struct sockaddr_in adrServ;
    adrServ.sin_addr.s_addr = inet_addr(argv[1]);
    adrServ.sin_family = AF_INET;
    adrServ.sin_port = htons(atoi(argv[2]));

    int lgAdr = sizeof(struct sockaddr_in);

    /* Etape 3 : envoyer une demande de connexion au serveur.*/
    int conn = connect(ds,(struct sockaddr *) &adrServ, lgAdr);
    // je traite les valeurs de retour
    if (conn <0){
        perror ("Client: pb au connect :");
        close (ds); // je libère les ressources avant de terminer.
        exit (1); // je choisi de quitter le programme : la suite dépend
        // de la réussite de l'envoi de la demande de connexion.
    }

    // je continue à alimenter le programme avec traces d'exécution
    printf("Client : demande de connexion reussie \n");


    // Je peux tester l'exécution de cette étape avant de passer à la suite.

    /* Etape 4 : envoyer un message au serveur. Ce message est une chaîne de caractères saisie au clavier. Vous pouvez utiliser une autre fonction pour la saisie. */

    int nbOctEchanges=0, nbSend=0;
    while(1){
        printf("saisir un message à envoyer (moins de 1500 caracteres) \n");
        printf("tapez quitter pour fermer la connection\n");
        char m[1501];
        fgets(m,sizeof(m), stdin); // copie dans m la chaîne saisie que
        // clavier (incluant les espaces et le
        // saut de ligne à la fin).
        m[strlen(m)-1]  = '\0'; // je retire le saut de ligne

        int taillemsg=strlen(m);
        int sndtaille=sendTCP(ds, (char*)&taillemsg, sizeof(taillemsg));
        nbSend++;
        nbOctEchanges+=sizeof(taillemsg);

        if(sndtaille<=0){
            close(ds);
            break;
        }

        int snd = sendTCP(ds, m, strlen(m));

        if(snd <= 0 ){
            close(ds);
            break;
        }

        nbSend++;
        nbOctEchanges+=strlen(m);
        printf("Client : j'ai déposé %d octets au total (message + taille)\n", nbOctEchanges);

        if(strcmp("quitter",m)==0){
            close(ds);
            break;
        }

        int reponse;
        int rcv = recvTCP (ds,(char*)&reponse,sizeof(int));
        if(rcv <= 0){
            close(ds);
            break;
        }

    }
    close (ds);
    /*On affiche le nombre total d'octets échangé et le nombre d'appel à sendTCP
     * avec le serveur au cas ou il s'arrête*/
    printf("J'ai échangé %d octet avec le serveur!\n",nbOctEchanges);
    printf("J'ai appelé %d fois la fonction sendTCP!\n",nbSend);
    printf("Client : je termine\n");

    return 0;
}
