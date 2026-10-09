CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11
TARGET = myls

OBJS = main.o listing.o options.o display.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c ls.h options.h
	$(CC) $(CFLAGS) -c main.c

listing.o: listing.c ls.h options.h display.h
	$(CC) $(CFLAGS) -c listing.c

options.o: options.c options.h
	$(CC) $(CFLAGS) -c options.c

display.o: display.c display.h options.h
	$(CC) $(CFLAGS) -c display.c

clean:
	rm -f $(OBJS) $(TARGET)
