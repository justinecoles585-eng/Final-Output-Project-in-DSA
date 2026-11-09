# Optional: "make" builds the program, "make clean" removes it.
# Without make, use:  g++ -std=c++11 -Wall -Wextra -o riffhouse main.cpp src/*.cpp

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
SOURCES = main.cpp $(wildcard src/*.cpp)

riffhouse: $(SOURCES) $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) -o riffhouse $(SOURCES)

clean:
	rm -f riffhouse
