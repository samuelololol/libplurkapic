CC = gcc
CFLAGS = -Wall -ggdb
SRCS = get.c post.c
OBJS = $(SRCS:.c=.o)
INC = -I/usr/include/libxml2 -I..
LD = -lcurl -lssl -lxml2
LIB_TARGET = ../librestful.a
TEST_TARGET = get post myxml

all: get post myxml

get: $(LIB_TARGET) get.c
	$(CC) $(CFLAGS) $(INC) get.c $(LIB_TARGET) -o $@ $(LD)

post: $(LIB_TARGET) post.c
	$(CC) $(CFLAGS) $(INC) post.c $(LIB_TARGET) -o $@ $(LD)

myxml: $(LIB_TARGET) myxml.c
	$(CC) $(CFLAGS) -Wno-unused-variable $(INC) myxml.c $(LIB_TARGET) -o $@ $(LD)

.PHONY: clean all distclean post get

clean:
	rm -rf $(TEST_TARGET)

distclean:
	make clean
	rm -rf *.png *.o a.out

