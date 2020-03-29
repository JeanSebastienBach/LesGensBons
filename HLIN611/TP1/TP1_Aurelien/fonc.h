#ifndef __FONC_H__
#define __FONC_H__
#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int sendTCP(int sock, char* msg, int sizeMsg);
int recvTCP(int sock, char* msg, int sizeMsg);

#endif
