#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SAMPLE_RATE (44100)
#define LATENCY_MS (60)
#define FRAMES_PER_BUFFER (SAMPLE_RATE * LATENCY_MS / 1000)
#define TRUE 1
#define PORT 54999
 
int main() {
  std::cout << "Listening on port "  << PORT << std::endl;
  struct sockaddr_in addr;
  socklen_t addrlen;
  int sock, status;
  struct ip_mreq mreq;
unsigned char buf[100000];
  static int so_reuseaddr = TRUE;


  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
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
        
        if (cv::waitKey(30) == 27) break; //May need to change to 100 
        status = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen);
        if (status < 0) {
            perror("recvfrom");
            break;
        }
        std::vector<uchar> data(buf, buf + status);
        cv::Mat frame = cv::imdecode(data, cv::IMREAD_COLOR);

        if (frame.empty()) {
           std::cerr << "Decoded frame is empty." << std::endl;
           continue;
        }

        cv::imshow("Receiver window", frame);

        if (cv::waitKey(30) == 27){
          break;
         }
     } 

  close(sock);
  cv::destroyAllWindows();
  return 0;
}
