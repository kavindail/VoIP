#include "client.h"
#include "portaudio.h"
#include <fstream>
#include <iostream>

// May need to change the sampling rate
#define SAMPLE_RATE (44100)

#define LATENCY_MS (2)
#define FRAMES_PER_BUFFER (SAMPLE_RATE * LATENCY_MS / 1000)

typedef struct {
  // std::ofstream *outputFile;  //
} UserData;

static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {

  UserData *data = (UserData *)userData;
  const float *in = (const float *)inputBuffer;

  // data->outputFile->write((const char *)inputBuffer,
  //                         framesPerBuffer * sizeof(float));

  sendDataToSocket((const char *)inputBuffer, framesPerBuffer * sizeof(float));
  std::cout << (const char *)inputBuffer;
  return paContinue;
}

int main() {
  // Initialize PortAudio
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    return 1;
  }

  // Declare PortAudio stream and parameters
  PaStream *stream;
  PaStreamParameters inputParameters;

  // Get the default input device
  inputParameters.device = Pa_GetDefaultInputDevice();
  if (inputParameters.device == paNoDevice) {
    std::cerr << "Error: No default input device." << std::endl;
    Pa_Terminate();
    return 1;
  }

  // Set input parameters for the stream
  inputParameters.channelCount = 1;         // mono input
  inputParameters.sampleFormat = paFloat32; // 32 bit floating point input
  inputParameters.suggestedLatency =
      Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  // Create an output file for writing binary data
  // std::ofstream outputFile("output.bin", std::ios::binary);
  // this line

  // Create user data structure and associate the output file with it
  UserData userData = {/*&outputFile*/}; //

  // Open a PortAudio stream with the specified parameters and callback function
  err = Pa_OpenStream(&stream, &inputParameters, NULL, SAMPLE_RATE,
                      FRAMES_PER_BUFFER, paClipOff, recordCallback, &userData);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    Pa_Terminate();
    return 1;
  }

  // Start the PortAudio stream
  err = Pa_StartStream(stream);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 1;
  }

  // Display a message indicating that recording has started
  std::cout << "Recording... Press 'Enter' to stop." << std::endl;
  std::cin.get();

  // Stop and close the PortAudio stream
  err = Pa_CloseStream(stream);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    Pa_Terminate();
    return 1;
  }

  // Close the output file
  // outputFile.close();  //

  Pa_Terminate();
  std::cout << "Recording finished." << std::endl;

  return 0;
}
