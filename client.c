/*Application that generates multicast messages*/
#include "client.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void sendDataToSocket(const char *inputBuffer, size_t dataSize, int port) {
  struct sockaddr_in addr;
  int addrlen, sock, status;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addrlen = sizeof(addr);
  addr.sin_addr.s_addr = inet_addr("230.0.0.1");

  status = sendto(sock, inputBuffer, dataSize, 0, (struct sockaddr *)&addr, addrlen);
  if (status < 0) {
    perror("sendto");
    exit(1);
  }
  close(sock);
}
