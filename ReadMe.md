## Backlog

## Server (Speaker): source of audio stream. server captures audio data, encodes it using a codec to reduce its size, then sends it over the network to clients.

## Clients (Listeners): recipients of audio stream. Each client receives the audio data from the server, decodes it, and plays the decoded audio.

Audio Encoding/Decoding (Codec):In VoIP, audio data is encoded to reduce its size for transmission, and then decoded on the client’s end.
Common codecs for this purpose include AAC, MP3, Opus, and others.

Streaming Protocol: This is the method used to send the audio data over the network. There are many protocols available for this purpose, but a common choice for audio streaming is the
Real-Time Streaming Protocol (RTSP).

Network: Will be using the internet with an IP address to send the data over

Serverside audio is recorded -> Sound -> Encode using a codec-> Binary Data -> Send over IP(Using RTSP)-> Client -> Decode using a codec -> Play audio

## Order of events

1. The server starts and waits for clients to connect.
2. Each client starts and connects to the server.
3. The server begins capturing audio data from the microphone.
4. The audio data is encoded using a codec.
5. The encoded audio data is sent to all connected clients using RTSP(Realtime streaming protocl).
6. Each client receives the audio data, decodes it (using a codec), and plays it through the speakers.
