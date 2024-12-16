#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class CompressionAlgorithm {

public:
  std::vector<float> decompressAudioData(const uint8_t *input,
                                         unsigned long size) {
    std::vector<float> decompressedData;

    for (unsigned long i = 0; i < size; ++i) {
      decompressedData.push_back(input[i] / 255.0f);
    }
    return decompressedData;
  }

  std::vector<uint8_t> compressAudioData(const float *input,
                                         unsigned long size) {
    std::vector<uint8_t> compressedData;
    for (unsigned long i = 0; i < size; ++i) {
      compressedData.push_back(static_cast<uint8_t>(input[i] * 255.0f));
    }
    return compressedData;
  }
};
