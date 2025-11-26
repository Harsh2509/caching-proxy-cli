CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
SRCS = main.cpp utils/cli_out.cpp utils/utils.cpp utils/validation.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = caching_proxy

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)