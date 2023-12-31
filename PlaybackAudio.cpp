#include "portaudio.h"
#include <fstream>
#include <iostream>

#define SAMPLE_RATE (44100)
#define LATENCY_MS (2)
#define FRAMES_PER_BUFFER (SAMPLE_RATE * LATENCY_MS / 1000)

void playRecordedAudio(const std::string &filename) {
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    return;
  }

  PaStream *stream;
  PaStreamParameters outputParameters;

  outputParameters.device = Pa_GetDefaultOutputDevice();
  if (outputParameters.device == paNoDevice) {
    std::cerr << "Error: No default output device." << std::endl;
    Pa_Terminate();
    return;
  }

  // Set output parameters for the stream
  outputParameters.channelCount = 1;         // mono output
  outputParameters.sampleFormat = paFloat32; // 32 bit floating point output
  outputParameters.suggestedLatency =
      Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE,
                      FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    Pa_Terminate();
    return;
  }

  std::ifstream inputFile(filename, std::ios::binary);

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    Pa_CloseStream(stream);
    Pa_Terminate();
    return;
  }

  const int bufferSize = FRAMES_PER_BUFFER;
  float buffer[bufferSize];

  while (inputFile.read((char *)buffer, bufferSize * sizeof(float))) {
    err = Pa_WriteStream(stream, buffer, bufferSize);
    if (err != paNoError) {
      std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
      break;
    }
  }

  std::streamsize bytesRead = inputFile.gcount() * sizeof(float);
  inputFile.seekg(0, std::ios::beg);
  inputFile.seekg(bytesRead, std::ios::cur);
  std::ofstream(filename, std::ios::binary | std::ios::trunc).close();

  err = Pa_StopStream(stream);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
  }

  err = Pa_CloseStream(stream);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
  }

  inputFile.close();

  Pa_Terminate();
}

int main() {
  while (1) {
    playRecordedAudio("output.bin");
  }

  return 0;
}
