#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  char buffer[50] = {0};
  struct sockaddr_in servaddr = {0};

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sockfd == -1) {
    std::cout << "Failed to create socket" << std::endl;
    exit(1);
  }
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(12345);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  int rc = bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

  if (rc == -1) {
    std::cout << "Failed to bind " << std::endl;
    close(sockfd);
    exit(1);
  };
  socklen_t len = 0;
  int n = recvfrom(sockfd, (char *)buffer, 50, MSG_WAITALL, 0,
                   &len); // 0 here is the client address
  // wait all specifies wait for the entire message to come

  buffer[n] = '\n';

  std::cout << buffer << std::endl;
  close(sockfd);

  return 0;
}
