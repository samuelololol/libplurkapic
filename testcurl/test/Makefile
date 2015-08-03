CC = gcc
CFLAGS = -Wall -ggdb -I..
SRCS = get.c post.c
OBJS = $(SRCS:.c=.o)
LD = -lcurl -lssl 
LIB_TARGET = ../librestful.a
TEST_TARGET = get post

all: get post

get: $(LIB_TARGET)
	$(CC) $(CFLAGS) get.c $(LIB_TARGET) -o $@ $(LD)

post: $(LIB_TARGET)
	$(CC) $(CFLAGS) post.c $(LIB_TARGET) -o $@ $(LD)


.PHONY: clean all distclean

clean:
	rm -rf $(TEST_TARGET)

distclean:
	make clean
	rm -rf *.png *.o


