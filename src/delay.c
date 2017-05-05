#include <time.h>
#include <stdio.h>
#include "delay.h"


int delaylength   = 50;    // number of iterations in delay()

void delay(int delaylength) 
{
  int i;
  float a = 0.;

  for (i = 0; i < delaylength; i++)
    a += i;
  // to avoid compiler optimization
  if (a < 0)
    printf("%f \n", a);
}

double reference()
{
  struct timespec start_ts, end_ts;
  clock_gettime(CLOCK_MONOTONIC, &start_ts);
  for (int i = 0; i < 1000; ++i) {
    delay(delaylength);
  }
  clock_gettime(CLOCK_MONOTONIC, &end_ts);
  return ((end_ts.tv_sec - start_ts.tv_sec) * 1E6
                + (end_ts.tv_nsec - start_ts.tv_nsec) / 1E3) / 1000;
}


