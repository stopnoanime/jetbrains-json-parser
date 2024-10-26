CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17

TARGET := main
SRCS := main.cpp lexer.cpp parser.cpp json.cpp query-lexer.cpp query-eval.cpp 
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