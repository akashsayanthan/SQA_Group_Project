CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET = distribution
SOURCES = main.cpp DistributionSystem.cpp
HEADERS = User.h Game.h DistributionSystem.h
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
