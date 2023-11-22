EXEC = VoiceOverIp
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
CXX = g++
CXXFLAGS = -I/opt/homebrew/Cellar/portaudio/19.7.0/include
LDFLAGS = -L/opt/homebrew/Cellar/portaudio/19.7.0/lib -lportaudio

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJ)
