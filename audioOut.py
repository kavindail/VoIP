import pyaudio

FILENAME = "output.bin"  # Name of the file to play back
CHUNK_SIZE = 1024  # Size of each audio chunk

# Audio format parameters
FORMAT = pyaudio.paFloat32
CHANNELS = 1
RATE = 44100

# Initialize PyAudio
p = pyaudio.PyAudio()

# Open the raw file in binary read mode
with open('output.bin', 'rb') as f:
    # Open a stream for audio playback
    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    output=True)

    # Read and play back chunks of data from the file
    data = f.read(CHUNK_SIZE)
    while data:
        stream.write(data)
        data = f.read(CHUNK_SIZE)

    # Close the stream
    stream.stop_stream()
    stream.close()

# Terminate PyAudio
p.terminate()
