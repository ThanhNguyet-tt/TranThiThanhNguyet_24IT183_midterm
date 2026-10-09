CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = myls

OBJS = main.o listing.o options.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c ls.h options.h
	$(CC) $(CFLAGS) -c main.c

listing.o: listing.c ls.h options.h
	$(CC) $(CFLAGS) -c listing.c

options.o: options.c options.h
	$(CC) $(CFLAGS) -c options.c

clean:
	rm -f $(OBJS) $(TARGET)
