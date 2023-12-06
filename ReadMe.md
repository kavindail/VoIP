## Voice Over Internet Protocol

Voice Over IP application that transmits data through UDP packets and played back in real time by the receiver
Meant to simulate a Voice Over IP telephone

## Compilation steps

These compilation steps are for Linux/Unix if on Windows or anything else idea is the same just compile with where the port audio library is located on your computer

VoiceOverIP Compilation
**_gcc -c client.c -o client.o_**
**_g++ -c RecordAudio.cpp -o RecordAudio.o -I/opt/homebrew/Cellar/portaudio/19.7.0/include_**
**_g++ client.o RecordAudio.o -o VoiceOverIp -L/opt/homebrew/Cellar/portaudio/19.7.0/lib -lportaudio_**
**_./VoiceOverIp_**

PlayBack Audio Compilation
**_g++ -I/opt/homebrew/Cellar/portaudio/19.7.0/include -c PlaybackAudio.cpp -o PlaybackAudio.o_**
**_g++ -o PlaybackAudio PlaybackAudio.o -L/opt/homebrew/Cellar/portaudio/19.7.0/lib -lportaudio_**
**_./PlaybackAudio_**

Server Compilation
**_gcc server.c -o server_**
**_./server_**
