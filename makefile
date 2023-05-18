CC = g++
CFLAGS = -std=c++11
TARGET = programa
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)