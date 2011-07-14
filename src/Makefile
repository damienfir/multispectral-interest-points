CC = gcc
CFLAGS = -c -Wall
LDFLAGS = -lm

EXECUTABLE = features
SOURCES = array.c sift.c ransac.c register.c tests.c dog.c harris.c main.c
OBJECTS = $(SOURCES:.c=.o)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) `pkg-config --libs opencv` $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
