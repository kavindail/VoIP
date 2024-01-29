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
