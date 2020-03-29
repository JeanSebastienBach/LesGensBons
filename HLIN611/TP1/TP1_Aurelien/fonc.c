#include "fonc.h"

int sendTCP(int sock, char* msg, int sizeMsg){
    int nbEnvoyer = 0;
    int nbsend = 0;
    while (nbEnvoyer < sizeMsg){
        nbsend = send(sock,msg+nbEnvoyer,sizeMsg-nbEnvoyer,0);
        switch(nbsend){
            case -1 :
                #ifdef __SERVEUR__
                perror("Serveur: erreur lors du send:");
                #else
                perror("Client: erreur lors du send:");
                #endif
                close(sock);
                return -1;
            case 0 :
                #ifdef __SERVEUR__
                printf("Serveur: client déconnecté\n")
                #else
                printf("Client: serveur déconnecté\n");
                #endif
                close(sock);
                return 0;
            default :
                nbEnvoyer = nbEnvoyer + nbsend;
                break;
        }
    }
    return 1;
}

int recvTCP(int sock, char* msg, int sizeMsg){
    int nbOctRecus=0;
    int nbrecv=0;
    while(nbOctRecus<sizeMsg){
        nbrecv=recv(sock, msg+nbOctRecus, sizeMsg-nbOctRecus,0);
        switch(nbrecv){
            case -1:
                #ifdef __SERVEUR__
                perror("Serveur: Erreur de reception");
                #else
                perror("Client: Erreur de reception");
                #endif
                return -1;
            case 0:
                #ifdef __SERVEUR__
                printf("Serveur déconnecté lors de la reception\n");
                #else
                printf("Client déconnecté lors de la reception\n");
                #endif
                return 0;
            default:
                nbOctRecus = nbOctRecus +nbrecv;
                break;
        }
    }
    return 1;
}
