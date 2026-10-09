CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = myls

OBJS = main.o listing.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c ls.h
	$(CC) $(CFLAGS) -c main.c

listing.o: listing.c ls.h
	$(CC) $(CFLAGS) -c listing.c

clean:
	rm -f $(OBJS) $(TARGET)
