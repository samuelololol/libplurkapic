CC = gcc
CFLAGS = -Wall -ggdb -Wno-unused-variable
SRCS = restful.c
OBJS = $(SRCS:.c=.o)
LD = -lcurl -lssl
TARGET = librestful.a

all: $(TARGET)

$(TARGET): $(OBJS)
	ar -rc $(TARGET) $(OBJS)
	ranlib $(TARGET)

restful.o: restful.c restful.h
	$(CC) $(CFLAGS) -c -o $@ $<

restful.c: restful.h

test: $(TARGET)
	make -C test all

.PHONY: clean all distclean test

clean:
	rm -rf $(TARGET) *.o

distclean:
	make -C test clean
	make clean
	rm -rf *.png


