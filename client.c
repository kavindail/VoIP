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

// Client is the program that sends data across the sockets
// This is where data will be transmitted in UDP sockets across the server

void sendDataToSocket(const char *inputBuffer) {
  struct sockaddr_in addr;
  int addrlen, sock, status;
  char buf[10000];
  time_t t;

  /* set up socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(4447);
  addrlen = sizeof(addr);
  addr.sin_addr.s_addr = inet_addr("230.0.0.1");

  /* Copy the content of inputBuffer to buf */
  snprintf(buf, sizeof(buf), "%s", inputBuffer);

  t = time(0);
  printf("%s", buf);

  /* Use the actual length of the data in buf */
  status = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&addr, addrlen);
  if (status < 0) {
    perror("sendto");
    exit(1);
  }

  close(sock); // Close the socket when done
}
