#include "CompressionAlgorithm.cpp"
#include "client.h"
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

#define PORT 55000
class PortAudioCallbacks {
public:
  static int recordCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {

    const float *in = (const float *)inputBuffer;
    CompressionAlgorithm compressor;
    auto compressedData = compressor.compressAudioData(in, framesPerBuffer);

    sendDataToSocket(reinterpret_cast<const char *>(compressedData.data()), compressedData.size(), PORT);
    std::cout << "Sending to port "  << PORT << std::endl;

    std::cout << "Original size: " << (framesPerBuffer * sizeof(float)) << ", Compressed size: " << compressedData.size() << std::endl;
    return paContinue;
  }

  static int playCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    float *out = (float *)outputBuffer;
    uint8_t *compressedData = (uint8_t *)userData;
    CompressionAlgorithm compressor;
    auto decompressedData = compressor.decompressAudioData(compressedData, 900);

    for (unsigned long i = 0; i < framesPerBuffer && i < decompressedData.size(); i++) {
       out[i] = decompressedData[i];
    }

    return paContinue;
  }
};
