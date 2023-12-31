#include "client.h"
#include "portaudio.h"
#include <fstream>
#include <iostream>

#define SAMPLE_RATE (44100)
#define LATENCY_MS (2)
#define FRAMES_PER_BUFFER (SAMPLE_RATE * LATENCY_MS / 1000)

static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {

  const float *in = (const float *)inputBuffer;

  sendDataToSocket((const char *)inputBuffer, framesPerBuffer * sizeof(float));
  std::cout << (const char *)inputBuffer;
  return paContinue;
}

int main() {
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
  inputParameters.suggestedLatency =
      Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream, &inputParameters, NULL, SAMPLE_RATE,
                      FRAMES_PER_BUFFER, paClipOff, recordCallback, 0);
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
