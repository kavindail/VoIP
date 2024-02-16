#include <iostream>
#include <cstring> // For memset
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h> // For inet_ntoa
#include <sys/socket.h> // For socket functions
#include <unistd.h> // For close
#include <thread> // For std::thread

void forward_packets(const char* listen_ip, int listen_port, int buffer_size) {
    int sock;
    struct sockaddr_in listen_addr, sender_addr;
    char buffer[buffer_size];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(listen_port);
    listen_addr.sin_addr.s_addr = inet_addr(listen_ip);

    if (bind(sock, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(sock);
        return;
    }

    std::cout << "Listening for incoming UDP packets on " << listen_ip << ":" << listen_port << std::endl;

    while (true) {
        socklen_t len = sizeof(sender_addr);

        ssize_t recv_len = recvfrom(sock, buffer, buffer_size, 0, (struct sockaddr*)&sender_addr, &len);
        if (recv_len < 0) {
            std::cerr << "recvfrom error" << std::endl;
            break;
        }

        std::cout << "Received packet from " << inet_ntoa(sender_addr.sin_addr) << ":" << ntohs(sender_addr.sin_port) << std::endl;



        if (sendto(sock, buffer, recv_len, 0, (struct sockaddr*)&sender_addr, len) < 0) {
            std::cerr << "sendto error" << std::endl;
            break;
        }

        std::cout << "Forwarded packet back to " << inet_ntoa(sender_addr.sin_addr) << ":" << ntohs(sender_addr.sin_port) << std::endl;
    }

    close(sock);
}

int main() {
    const char *listenIP = "0.0.0.0";
    int voicePortListen = 55000;
    int videoPortListen = 54999;
    int voiceBufferSize = 5700;
    int videoBufferSize = 10000;

    std::thread voiceThread(forward_packets, listenIP, voicePortListen, voiceBufferSize);
    std::thread videoThread(forward_packets, listenIP, videoPortListen, videoBufferSize);

    voiceThread.join();
    videoThread.join();

    return 0;
}
