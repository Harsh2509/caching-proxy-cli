CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
LDLIBS = -lcurl -lhiredis
SRCS = src/main.cpp src/utils/cli_out.cpp src/utils/utils.cpp src/utils/validation.cpp src/utils/api_factory.cpp src/services/redis_service.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = caching_proxy

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)