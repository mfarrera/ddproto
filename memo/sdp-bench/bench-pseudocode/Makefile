CC=gcc
CFLAGS=-I.
DEPS = grid.h

TARGET= simple-imaging 
OBJECTS= simple-imaging.o grid.o hdf5.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

TARGET: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *~ core 
