CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRCS = main.c commands.c variables.c io.c
OBJS = $(SRCS:.c=.o)
TARGET = sosi

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
