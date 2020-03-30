#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

int receiveTCP(int sock, char * msg, int sizeMsg)
{
  int nbReceivedOctets = 0;
  while (nbReceivedOctets < sizeMsg)
  {
    int nbReceived = recv(sock, msg + nbReceivedOctets, sizeMsg - nbReceivedOctets, 0);//même principe que pour sendTCP
    if (nbReceived <= 0)
    {
      return nbReceived;
    }
    else
    {
      nbReceivedOctets += nbReceived;
    }
    return 1;
  }
}

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("[Server] Wrong usage : you must type %s serverPort !\n", argv[0]);
    exit(1);
  }

  int ds = socket(PF_INET, SOCK_STREAM, 0);
  if (ds == -1)
  {
    perror("[Server] Error while creating socket");
    exit(1);
  }

  printf("[Server] Socket successfully created.\n");

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1]));

  if (bind(ds, (struct sockaddr *)&server, sizeof(server)) < 0) // binding socket
  {
    perror("[Server] Error while binding socket");
    close(ds);
    exit(1);
  }

  printf("[Server] Binding successful.\n");

  int ecoute = listen(ds, 5);
  if (ecoute < 0)
  {
    printf("[Server] Error: cannot listen to the socket");
    close(ds);
    exit(1);
  }
  printf("[Server] Listening to the socket...\n");

  printf("[Server] Waiting for a client... (accept) \n");

  struct sockaddr_in adCv;
  socklen_t lgCv = sizeof(struct sockaddr_in);
  int nbRecvOctTotal = 0, nbRecvMessTotal = 0, dsCv;

  dsCv = accept(ds, (struct sockaddr *)&adCv, &lgCv);
  if (dsCv < 0)
  {
    perror("[Server] Error : could not connect to client");
    close(ds);
    exit(1);
  }
    printf("[Server] Client %s:%d connected.\n", inet_ntoa(adCv.sin_addr), ntohs(adCv.sin_port));

  while (1)
  {
    int nbRecvOct = 0;


    int length = 0;
    int rcv = receiveTCP(dsCv, (char *)&length, sizeof(int));// reception de la tailles des futurs messages

    if (rcv < 0)
    {
      perror("[Server] Error : Could not get message length, ending connection with client");
      close(dsCv);
      break;
    }
    if (rcv == 0)
    {
      perror("[Server] Client left, ending connection");
      close(dsCv);
      break;
    }
    printf("[Server] %d octet(s) must be received. \n", length);
    /*Je compte le message indicant la taille des messages dans les octets/messages échangés*/
    nbRecvOct += sizeof(int);
    nbRecvMessTotal++;

    char *buffer = malloc(sizeof(char) * (length + 1));
    rcv = receiveTCP(dsCv, buffer, length); // reception du message
    
    
    //gestion des erreurs
    if (rcv < 0)
    {
      perror("[Server] Error : Could not get message length, ending connection with client");
      close(dsCv);
      break;
    }
    if (rcv == 0)
    {
      perror("[Server] Client left, ending connection");
      close(dsCv);
      break;
    }
    nbRecvOct += length;
    nbRecvMessTotal++;
    printf("[Server] %d octet(s) has been received (length + content).\n", nbRecvOct);
    printf("[Server] Message content : \"%s\" \n", buffer);
    free(buffer);
    nbRecvOctTotal += nbRecvOct;
    int snd = send(dsCv, &nbRecvOct, sizeof(int), 0);
    
    
    //gestion des erreurs
    if (snd < 0)
    {
      perror("[Server] Error: could not send validation\n");
      close(ds);
      break;
    }

    if (snd == 0)
    {
      perror("[Server] Error : could not send validation, client is not connected \n");
      close(ds);
      break;
    }
  }

  close(ds);
  printf("[Server] Connection ended.\n");
  printf("[Server] A total of %d octet(s) were sent during this connection. \n", nbRecvOctTotal);
  printf("[Server] The fonction 'receiveTCP()' was used %d times. \n", nbRecvMessTotal-1);
}