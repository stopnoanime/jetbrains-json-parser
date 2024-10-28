CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -g

TARGET := main
SRCS := $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 

run: $(TARGET)
	./main