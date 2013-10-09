TARGET = sl
LIBS = -lm
CC = gcc
CFLAGS =-Wall
EXEOBJ = main.o
LIBSOBJ = sorted-list.o
LIBSL = libsl.a

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBSL): $(LIBSOBJ)
	ar rc $(LIBSL) $(LIBSOBJ)

$(TARGET): $(EXEOBJ) $(LIBSL)
	$(CC) $(CFLAGS) $(LIBS) sorted-list.c main.c sorted-list.h -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f $(LIBSL)
	-rm -f *~
