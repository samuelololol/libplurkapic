CC = gcc
CFLAGS = -Wall -ggdb
SRCS = restful.c
OBJS = $(SRCS:.c=.o)
LD = -lcurl -lssl
LIB_TARGET = librestful.a
TEST_TARGET = http

all: $(LIB_TARGET)

$(LIB_TARGET): $(OBJS)
	ar -rc $(LIB_TARGET) $(OBJS)
	ranlib $(LIB_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

restful.c: restful.h

test: $(LIB_TARGET)
	$(CC) $(CFLAGS) hello.c $(LIB_TARGET) -o $(TEST_TARGET) $(LD)

.PHONY: clean all distclean test

clean:
	rm -rf $(TEST_TARGET) $(LIB_TARGET) *.o

distclean:
	make clean
	rm -rf *.png


