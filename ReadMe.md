## Voice Over Internet Protocol

Voice Over IP application that transmits data through UDP packets and played back in real time by the receiver
Meant to simulate a Voice Over IP telephone

## Compilation steps

These compilation steps are for Linux/Unix if on Windows or anything else idea is the same just compile with where the port audio library is located on your computer

VoiceOverIP Compilation

gcc -c client.c -o client.o
g++ -c RecordAudio.cpp -o RecordAudio.o -I/opt/homebrew/Cellar/portaudio/19.7.0/include
g++ client.o RecordAudio.o -o VoiceOverIp -L/opt/homebrew/Cellar/portaudio/19.7.0/lib -lportaudio
./VoiceOverIp

**Server and playback audio compilation**

g++ -I/opt/homebrew/Cellar/portaudio/19.7.0/include -c server.cpp -o PlaybackAudio.o
g++ -o PlaybackAudio PlaybackAudio.o -L/opt/homebrew/Cellar/portaudio/19.7.0/lib -lportaudio
./PlaybackAudio


**Compile Record Video**

g++ RecordVideo.cpp client.c -o RecordVideo -I/opt/homebrew/Cellar/opencv/4.9.0_2/include/opencv4 -L/opt/homebrew/Cellar/opencv/4.9.0_2/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs -std=c++11

**Compile Receive Video**

g++ ReceiveVideo.cpp -o ReceiveVideo -I/opt/homebrew/Cellar/opencv/4.9.0_2/include/opencv4 -L/opt/homebrew/Cellar/opencv/4.9.0_2/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs -std=c++11


**Or Run build script**

chmod +x build_voip.sh

./build_voip.sh


