CC=gcc
CFLAGS=-I. -I/data/pw410/usr/fftw/3.3.4/include 
LDFLAGS= -L/data/pw410/usr/fftw/3.3.4/lib -lfftw3 -lm
DEPS = grid.h

TARGET= simple-imaging 
OBJECTS= simple-imaging.o grid.o hdf5.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

TARGET: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o *~ core 
