#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>


int sendTCP(int sock, char * msg, int sizeMsg)
{
   int nbSentSucceed = 0;
   int nbSentTotal = 0;
   while (nbSentSucceed < sizeMsg)
   {
      nbSentSucceed = send(sock, msg + nbSentTotal, sizeMsg - nbSentTotal, 0); // On utilise le total pour pas ré-envoyer de l'information.

      if (nbSentSucceed <= 0)
      {
         return nbSentSucceed; // S'il a une erreur, le code d'erreur est renvoyé.
      }
      else
      {
         nbSentTotal += nbSentSucceed; // Compabilisation du nombre d'octets déjà retranscrits.
      }
      return 1;
   }
}

int main(int argc, char *argv[])
{

   if (argc != 4)
   {
      printf("[Client] Wrong usage : you must type %s serverIP serverPort nbIteration !\n", argv[0]);
      exit(0);
   }

   int ds = socket(PF_INET, SOCK_STREAM, 0);

//gestion des erreurs
   if (ds == -1)
   {
      perror("[Client] Error while creating socket \n");
      exit(1);
   }

   printf("[Client] Socket successfully created.\n");

   struct sockaddr_in adrServ;
   adrServ.sin_addr.s_addr = inet_addr(argv[1]);
   adrServ.sin_family = AF_INET;
   adrServ.sin_port = htons((short)atoi(argv[2]));
   int nbSent = 0;
   int lgAdr = sizeof(struct sockaddr_in);

   int conn = connect(ds, (struct sockaddr *)&adrServ, lgAdr);
   //gestion des erreurs
   if (conn < 0)
   {
      perror("[Client] Error: could not connect to server");
      close(ds);
      exit(1);
   }
   printf("[Client] Connection successful. \n");

   printf("[Client] Please enter a message (1500 characters max)\n");
   char m[1501];
   fgets(m, sizeof(m), stdin);
   printf("Your message will be send %d times \n", atoi(argv[3]));
   m[strlen(m) - 1] = '\0';
   int totalOct = 0;
   int length = strlen(m);
   while (nbSent <= atoi(argv[3]))
   {
      int sndL = sendTCP(ds, &length, sizeof(length));
      //gestion des erreurs
      if (sndL < 0)
      {
         perror("[Client] Error while sending message length");
         close(ds);
         break;
      }
      else if (sndL == 0)
      {
         printf("[Client] Unable to reach server. \n");
         close(ds);
         break;
      }
         int nbOct;
         int snd = sendTCP(ds, m, strlen(m));

        //gestion des erreurs
         if (snd < 0)
         {
            perror("[Client] Error: could not send \n");
            printf("%d messages were sent. \n", nbSent);
            close(ds);
            break;
         }

         if (snd == 0)
         {
            perror("[Client] Error : server did not receive, client is not connected \n");
            printf("%d messages were sent. \n", nbSent);
            close(ds);
            break;
         }

         nbOct = strlen(m);
         totalOct += nbOct;

         printf("[Client] Successfully sent %d octet(s). [%d] \n", nbOct, nbSent);
         nbSent++;

         printf("[Client] Waiting for server's response...\n");

         int response;
         int rcv = recv(ds, &response, sizeof(int), 0);

        
        //gestion des erreurs
         if (rcv < 0)
         {
            perror("[Client] Error : could not receive server's response\n");
            close(ds);
            exit(1);
         }

         if (rcv == 0)
         {
            perror("[Client] Server is not connected. \n");
            close(ds);
            break;
         }

         if (rcv < sizeof(int))
         {
            printf("[Client] Error: could not ensure the integrity of server's response \n");
            close(ds);
            break;
         }

         printf("[Client] Successfully sent %d octet(s), server has received : %d octet(s). \n", nbOct, response);
      }

      close(ds);
      printf("[Client] Connection ended.\n");
      printf("[Client] A total of %d octet(s) were sent during this connection. \n", totalOct);
      printf("[Client] The fonction 'sendTCP()' was used %d times. \n", nbSent-1);
   }
