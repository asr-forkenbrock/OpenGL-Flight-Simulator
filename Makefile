CXX = g++
CXXFLAGS = -std=c++17 -Wall
LIBS = -lGLEW -lglfw -lGL
TARGET = flight-sim
SRCS = main.cpp graphic_tools_2d.cpp Simulation.cpp Shaders.cpp game_config.cpp Sim_Setup.cpp HUD.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)