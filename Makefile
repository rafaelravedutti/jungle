CC=gcc
CFLAGS=-I. -O3
DEPS = jungle.h tree.h
OBJ = main.o tree.o jungle.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o
