#include "PortAudioCallbacks.cpp"
#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define SAMPLE_RATE (6000)
#define LATENCY_MS (1000)
#define FRAMES_PER_BUFFER (SAMPLE_RATE * LATENCY_MS / 1000)
#define TRUE 1
#define PORT 4447

int main() {
  struct sockaddr_in addr;
  socklen_t addrlen;
  int sock, status;
  struct ip_mreq mreq;
  float buf[FRAMES_PER_BUFFER];
  static int so_reuseaddr = TRUE;
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
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
  // outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, callback.playCallback, buf);
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

  mreq.imr_multiaddr.s_addr = inet_addr("230.0.0.1");
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    perror("setsockopt mreq");
    exit(1);
  }

  addrlen = sizeof(addr);
  while (1) {
    status = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen);
    if (status < 0) {
      perror("recvfrom");
      break;
    }
  }

  Pa_StopStream(stream);
  Pa_CloseStream(stream);
  Pa_Terminate();
  close(sock);

  return 0;
}
