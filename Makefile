OBJECTS:= BigN.o
CC := gcc -std=gnu99 -O2

all:
	$(MAKE) test

test: test.c $(OBJECTS)
	$(CC) -o $@ $^
	./test

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm $(OBJECTS)
	rm *.o

