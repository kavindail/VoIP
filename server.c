#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TRUE 1

int main() {
  struct sockaddr_in addr;
  int addrlen, sock, status;
  struct ip_mreq mreq;
  char buf[500];
  static int so_reuseaddr = TRUE;

  FILE *outputFile = fopen("output.bin", "wb");
  if (outputFile == NULL) {
    perror("Error opening output.bin for writing");
    exit(1);
  }

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(4447);
  addrlen = sizeof(addr);

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr,
                 sizeof(so_reuseaddr)) < 0) {
    perror("setsockopt reuse addr");
    exit(1);
  }

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }

  mreq.imr_multiaddr.s_addr = inet_addr("230.0.0.1");
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);

  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) <
      0) {
    perror("setsockopt mreq");
    exit(1);
  }

  while (1) {
    status =
        recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen);

    if (status < 0) {
      perror("recvfrom");
      exit(1);
    }

    size_t written = fwrite(buf, sizeof(char), status, outputFile);

    if (written != (size_t)status) {
      printf("failed: %zu of %d bytes written\n", written, status);
      exit(1);
    }

    fflush(outputFile);
    fwrite(buf, sizeof(char), status, stdout);
  }

  fclose(outputFile);
  return 0;
}
