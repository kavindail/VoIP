#include "PortAudioCallbacks.cpp"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
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

int main(int argc, char *argv[]) {
  std::map<std::string, std::string> args;

  for (int i = 1; i < argc - 1; i += 2) {
    std::string arg = argv[i];
    std::string value = argv[i + 1];
    args[arg] = value;
  }
  int listenPort;

  try {
    if (args.find("--listen-port") != args.end()) {
      listenPort = std::stoi(args["--listen-port"]);
    } else {
      std::cout << "Missing --listen-port" << std::endl;
      exit(1);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error parsing arguments: " << e.what() << std::endl;
    std::cerr << "eg: " << argv[0] << " --listen-port port" << std::endl;
    std::cerr << "eg: " << argv[0] << " --listen-port 8080" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Listening to port " << listenPort << std::endl;

  struct sockaddr_in addr;
  socklen_t addrlen;
  int sock, status;
  char packetBuffer[FRAMES_PER_BUFFER * sizeof(float) + 64];
  float audioBuffer[FRAMES_PER_BUFFER];
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
  addr.sin_port = htons(listenPort);
  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind failed");
    close(sock);
    exit(EXIT_FAILURE);
  }

  std::cout << "Waiting for data..." << std::endl;
  addrlen = sizeof(addr);
  while (1) {
    status = recvfrom(sock, packetBuffer, sizeof(packetBuffer), 0,
                      (struct sockaddr *)&addr, &addrlen);
    if (status < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        usleep(10000);
        continue;
      } else {
        perror("recvfrom error");
        break;
      }
    }

    if (strncmp(packetBuffer, "VOICE:", 6) == 0) {
      std::cout << "Received VOICE packet" << std::endl;
      size_t dataSize = status - 6;
      if (dataSize > sizeof(audioBuffer)) {
        dataSize = sizeof(audioBuffer);
      }
      memcpy(audioBuffer, packetBuffer + 6, dataSize);
    } else {
      std::cout << "Ignoring non-VOICE packet" << std::endl;
      sleep(1);
    }
  }

  Pa_StopStream(stream);
  Pa_CloseStream(stream);
  Pa_Terminate();
  close(sock);

  return 0;
}
