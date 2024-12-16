#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define PORT 47092

int main() {
  std::cout << "Listening on port " << PORT << std::endl;
  struct sockaddr_in addr;
  socklen_t addrlen;
  int sock, status;
  const int bufSize = 100000;
  unsigned char buf[bufSize];
  static int so_reuseaddr = 1;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr,
                 sizeof(so_reuseaddr)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
    exit(EXIT_FAILURE);
  }

  int flags = fcntl(sock, F_GETFL, 0);
  if (flags < 0) {
    perror("fcntl(F_GETFL) failed");
    exit(EXIT_FAILURE);
  }
  flags |= O_NONBLOCK;
  if (fcntl(sock, F_SETFL, flags) < 0) {
    perror("fcntl(F_SETFL) failed");
    exit(EXIT_FAILURE);
  }

  int so_reuseport = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &so_reuseport,
                 sizeof(so_reuseport)) < 0) {
    perror("setsockopt(SO_REUSEPORT) failed");
    exit(EXIT_FAILURE);
  }

  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(PORT);
  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind failed");
    close(sock);
    exit(EXIT_FAILURE);
  }

  while (true) {
    addrlen = sizeof(addr);
    status =
        recvfrom(sock, buf, bufSize, 0, (struct sockaddr *)&addr, &addrlen);
    if (status < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        usleep(10000);
        continue;
      } else {
        perror("recvfrom error");
        break;
      }
    }

    std::string header(reinterpret_cast<char *>(buf), 6);
    if (header == "VIDEO:") {
      std::vector<uchar> data(buf + 6, buf + status);
      cv::Mat frame = cv::imdecode(data, cv::IMREAD_COLOR);

      if (frame.empty()) {
        std::cerr << "Decoded frame is empty." << std::endl;
        continue;
      }

      cv::imshow("Receiver window", frame);
    } else {
      std::cerr << "Received packet is not a video frame." << std::endl;
      continue;
    }
  }

  close(sock);
  cv::destroyAllWindows();
  return 0;
}
