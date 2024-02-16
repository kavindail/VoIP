#include "PortAudioCallbacks.cpp"
#include <fcntl.h>
#include "client.h"
#include "portaudio.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#define SAMPLE_RATE (3000)
#define LATENCY_MS (300)
#define FRAMES_PER_BUFFER (SAMPLE_RATE * LATENCY_MS / 1000)

int main() {
  PortAudioCallbacks callback;
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    return 1;
  }
  PaStream *stream;
  PaStreamParameters inputParameters;

  inputParameters.device = Pa_GetDefaultInputDevice();
  if (inputParameters.device == paNoDevice) {
    std::cerr << "Error: No default input device." << std::endl;
    Pa_Terminate();
    return 1;
  }

  inputParameters.channelCount = 1;
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
  // inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream, &inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, callback.recordCallback, 0);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    Pa_Terminate();
    return 1;
  }

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 1;
  }

  std::cout << "Recording... Press Enter to stop." << std::endl;
  std::cin.get();

  err = Pa_CloseStream(stream);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    Pa_Terminate();
    return 1;
  }

  Pa_Terminate();
  std::cout << "Recording finished." << std::endl;

  return 0;
}
