CC=gcc
CFLAGS=-g -Wall -Werror
OBJS=cfire.o
TARGET=cfire

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	-rm -f $(OBJS) $(TARGET) *~
