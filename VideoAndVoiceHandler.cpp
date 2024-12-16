#include "PortAudioCallbacks.cpp"
#include <arpa/inet.h>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <opencv2/opencv.hpp>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define SAMPLE_RATE (3000)
#define LATENCY_MS (300)
#define FRAMES_PER_BUFFER (SAMPLE_RATE * LATENCY_MS / 1000)
#define PORT 47092

int main() {
  std::cout << "Listening to port " << PORT << std::endl;
  struct sockaddr_in addr;
  socklen_t addrlen;
  int sock, status;
  char packetBuffer[FRAMES_PER_BUFFER * sizeof(float) + 64000];
  const int bufSize = 1000000;
  float audioBuffer[FRAMES_PER_BUFFER];
  unsigned char buf[bufSize];
  static int so_reuseaddr = 1;
  PortAudioCallbacks callback;
  PaStreamParameters outputParameters;
  PaStream *stream;
  PaError err;

  err = Pa_Initialize();
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    exit(1);
  }

  outputParameters.device = Pa_GetDefaultOutputDevice();
  if (outputParameters.device == paNoDevice) {
    fprintf(stderr, "Error: No default output device.\n");
    exit(1);
  }
  outputParameters.channelCount = 1;
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency =
      Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE,
                      FRAMES_PER_BUFFER, paClipOff, callback.playCallback,
                      audioBuffer);

  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    exit(1);
  }

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    exit(1);
  }

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

  std::cout << "Waiting for data..." << std::endl;
  while (1) {
    status = recvfrom(sock, packetBuffer, sizeof(packetBuffer), 0,
                      (struct sockaddr *)&addr, &addrlen);
    addrlen = sizeof(addr);
    std::string header(reinterpret_cast<char *>(packetBuffer), 6);
    if (status > 6) {
      if (strncmp(packetBuffer, "VOICE:", 6) == 0) {
        std::cout << "Received VOICE packet" << std::endl;
        size_t dataSize = status - 6;
        if (dataSize > sizeof(audioBuffer)) {
          dataSize = sizeof(audioBuffer);
        }
        memcpy(audioBuffer, packetBuffer + 6, dataSize);
      } else if (strncmp(packetBuffer, "VIDEO:", 6) == 0) {
        std::cout << "Received VIDEO packet" << std::endl;
        std::vector<uchar> data(packetBuffer + 6, packetBuffer + status);
        cv::Mat frame = cv::imdecode(data, cv::IMREAD_COLOR);
        if (frame.empty()) {
          std::cerr << "Decoded frame is empty." << std::endl;
          continue;
        } else {
          cv::imshow("Receiver window", frame);
          cv::waitKey(1);
        }
      } else {
      }
    } else {
    }
  }

  Pa_StopStream(stream);
  Pa_CloseStream(stream);
  Pa_Terminate();
  cv::destroyAllWindows();
  close(sock);

  return 0;
}
