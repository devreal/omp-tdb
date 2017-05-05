# Uncomment the following line to use OpenMP 2.0 features
#OMPFLAG = -DOMPVER2
# Uncomment the following line to use OpenMP 3.0 features
OMPFLAG = -DOMPVER2 -DOMPVER3

CC=cc 
CXX=CC -std=c++11
#CFLAGS = -O1 -lm  -DWITH_PAPI -fopenmp
CFLAGS = -O1 -lm -fopenmp
LDFLAGS = -O0 -lm -fopenmp
CPP = /usr/bin/cpp
LIBS = 
