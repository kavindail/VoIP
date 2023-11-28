# Makefile for compiling RecordAudio.cpp and server.c

# Executable name
EXEC := VoiceOverIp

# C++ Source files
CPP_SRC := RecordAudio.cpp

# C Source files
C_SRC := server.c

# Object files
OBJ := $(CPP_SRC:.cpp=.o) $(C_SRC:.c=.o)

# Compiler settings
CXX := g++
CC := gcc
CXXFLAGS := -I/opt/homebrew/Cellar/portaudio/19.7.0/include
CFLAGS := -I/opt/homebrew/Cellar/portaudio/19.7.0/include
LDFLAGS := -L/opt/homebrew/Cellar/portaudio/19.7.0/lib -lportaudio

# Rule to make the executable
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Rule to compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(EXEC) $(OBJ)

.PHONY: clean
