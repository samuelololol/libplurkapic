CC = gcc
CFLAGS = -Wall -ggdb -I..
SRCS = get.c
OBJS = $(SRCS:.c=.o)
LD = -lcurl -lssl
LIB_TARGET = ../librestful.a
TEST_TARGET = get

all: get

get: $(LIB_TARGET)
	$(CC) $(CFLAGS) get.c $(LIB_TARGET) -o $(TEST_TARGET) $(LD)

.PHONY: clean all distclean test

clean:
	rm -rf $(TEST_TARGET) $(LIB_TARGET) *.o

distclean:
	make clean
	rm -rf *.png


