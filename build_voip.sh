
#!/bin/bash

set -e

PORTAUDIO_PATH="/opt/homebrew/Cellar/portaudio/19.7.0"
OPENCV_PATH="/opt/homebrew/Cellar/opencv/4.9.0_2"

echo "Compiling client..."
gcc -c client.c -o client.o

echo "Compiling RecordAudio..."
g++ -c RecordAudio.cpp -o RecordAudio.o -I${PORTAUDIO_PATH}/include
g++ client.o RecordAudio.o -o VoiceOverIp -L${PORTAUDIO_PATH}/lib -lportaudio

echo "Compiling Server and PlaybackAudio..."
g++ -I${PORTAUDIO_PATH}/include -c server.cpp -o PlaybackAudio.o
g++ -o PlaybackAudio PlaybackAudio.o -L${PORTAUDIO_PATH}/lib -lportaudio

echo "Compiling RecordVideo..."
g++ RecordVideo.cpp client.c -o RecordVideo -I${OPENCV_PATH}/include/opencv4 -L${OPENCV_PATH}/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs -std=c++11

echo "Compiling ReceiveVideo..."
g++ ReceiveVideo.cpp -o ReceiveVideo -I${OPENCV_PATH}/include/opencv4 -L${OPENCV_PATH}/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs -std=c++11

echo "Compilation complete.\n----------------------------\nRun\n./VoiceOverIp\n./PlaybackAudio\n./RecordVideo\n./ReceiveVideo\n----------------------------"
