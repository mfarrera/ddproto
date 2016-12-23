PROGRAM=heat-mpi-ompss-p

export IFS=";"

THREADS="01;02;03;04;05;06;07;08;09;10;11;12"
INPUT=test.dat

for thread in $THREADS; do
  NX_THREADS=$thread mpirun ./$PROGRAM $INPUT test.ppm
done
