CC = gcc
CFLAGS = -c -Wall
LDFLAGS = -lm

EXECUTABLE = features
SOURCES = harris.c array.c main.c
OBJECTS = $(SOURCES:.c=.o)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) `pkg-config --libs opencv` $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
