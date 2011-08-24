LIBNAME = libfeatures.so
SOURCES = array.c sift.c ransac.c register.c dog.c harris.c
OBJECTS = $(SOURCES:.c=.o)

CC = gcc
CFLAGS = -c -Wall -fPIC
LDFLAGS = -lm -shared -Wl,-soname,$LIBNAME

all: $(LIBNAME)

$(LIBNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) `pkg-config --libs opencv` $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(LIBNAME)
