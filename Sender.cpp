#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  std::string hello = "Hello from client";
  struct sockaddr_in servaddr = {0};
  // Sock dgram since UDP packet
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket" << std::endl;
    exit(1);
  }
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = 8083;
  servaddr.sin_addr.s_addr = inet_addr("10.65.75.126"); // Equivelant of 0.0.0.0

  int len = sendto(sockfd, hello.c_str(), hello.size(), 0,
                   (const struct sockaddr *)&servaddr, sizeof(servaddr));
  if (len == -1) {
    std::cout << "Failed to send message" << std::endl;
    exit(1);
  }
  close(sockfd);
};
