TARGET = s1
LIBS = -lm
CC = gcc
CFLAGS =-ansi -pedantic -Wall

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@
	ar -r libs1.a $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f libs1.a
	-rm -f *~
