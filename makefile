CC = g++
CFLAGS = -std=c++11 -Wall
TARGET = programa
SRCS = Binario.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
CSV_URL = https://drive.google.com/uc\?export\=download\&id\=1i_HnleotJ1pg0nJgn8-cPD7eMAxvSjI_

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:	
	rm -f $(OBJS) $(TARGET)

download_csv:
	curl -o data_athlete_info.csv -L $(CSV_URL)