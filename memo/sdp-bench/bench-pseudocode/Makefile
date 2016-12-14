CC=gcc
#CFLAGS=-I. -I/data/pw410/usr/fftw/3.3.4/include 
#LDFLAGS= -L/data/pw410/usr/fftw/3.3.4/lib -lfftw3 -lm
CFLAGS=-I. 
LDFLAGS= -lfftw3 -lm -L/usr/local/Cluster-Apps/hdf5/nompi/1.8.16/lib -lhdf5
DEPS = grid.h

TARGET=simple-imaging 
OBJECTS= simple-imaging.o grid.o hdf5.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o *~ core $(TARGET)
