cmake_minimum_required(VERSION 3.12)
project(VoiceOverIp)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add the executable
add_executable(recordAudio RecordAudio.cpp)

add_executable(playbackAudio PlaybackAudio.cpp)

# Include and Link PortAudio
target_include_directories(recordAudio PRIVATE /opt/homebrew/Cellar/portaudio/19.7.0/include)
target_link_libraries(recordAudio PRIVATE /opt/homebrew/Cellar/portaudio/19.7.0/lib/libportaudio.dylib)


target_include_directories(playbackAudio PRIVATE /opt/homebrew/Cellar/portaudio/19.7.0/include)
target_link_libraries(playbackAudio PRIVATE /opt/homebrew/Cellar/portaudio/19.7.0/lib/libportaudio.dylib)

