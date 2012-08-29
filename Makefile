JSON_C_DIR=/usr/local/Cellar/json-c/0.9
DEBUG := -DSAMUEL_DEBUG
#DEBUG := 
CFLAGS += -ggdb -Wall $(DEBUG)
LDFLAGS+= -lssl -loauth -lcurl 
CFLAGS += -I$(JSON_C_DIR)/include/json
LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson

all: libfinchurk.o main.o
	gcc $(CFLAGS) $(LDFLAGS) -o test.out main.o libfinchurk.o

libfinchurk.o: libfinchurk.c
	gcc $(CFLAGS) $(LDFLAGS) -o libfinchurk.o -c libfinchurk.c

main.o: main.o
	gcc $(CFLAGS) -o main.o -c main.c
	
clean:
	rm -rf *.o test.out
