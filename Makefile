JSON_C_DIR=/usr/local/Cellar/json-c/0.9
DEBUG := -DSAMUEL_DEBUG
#DEBUG := 
CFLAGS += -ggdb -Wall $(DEBUG)
LDFLAGS+= -lssl -loauth -lcurl 
CFLAGS += -I$(JSON_C_DIR)/include/json
LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson-c

all: libplurkapic.o main.o
	gcc $(CFLAGS) $(LDFLAGS) -o test.out main.o libplurkapic.o
	#doxygen Doxygen

libplurkapic.o: libplurkapic.c
	gcc $(CFLAGS) -o libplurkapic.o -c libplurkapic.c

main.o: main.c
	gcc $(CFLAGS) -o main.o -c main.c
	
vercode: vercode.c
	gcc $(CFLAGS) $(LDFLAGS) $^ -o vercode

clean:
	rm -rf *.o test.out docs vercode
