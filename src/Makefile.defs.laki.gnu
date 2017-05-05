# Uncomment the following line to use OpenMP 2.0 features
#OMPFLAG = -DOMPVER2
# Uncomment the following line to use OpenMP 3.0 features
OMPFLAG = -DOMPVER2 -DOMPVER3

CC=gcc 
CXX=g++ -std=c++11
CFLAGS =  -O1 -lm -fopenmp -DWITH_PAPI -ggdb
LDFLAGS = -O0 -lpapi -lm -fopenmp -L/sw/laki-SL6x/hlrs/performance/papi/2015-09-09-git85a9312-gnu-5.2.0/lib -lpapi
CPP = /usr/bin/cpp
LIBS = 
