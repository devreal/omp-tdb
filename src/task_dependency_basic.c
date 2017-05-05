#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "delay.h"

#define MAX_DEPS 32 // the maximum number of dependencies a task can have

#define TASK_GRAPH_WIDTH 100

static char *buf1 = 0xDEADBEEFL;
static char *buf2 = 0xCAFEBABE;

static char *depbuf = (char*) 0xDEADBEEFL;

int innerreps     = 100;    // number of inner repetitions
int numberoftasks = 1000;  // number of tasks per iterations

void benchmark_stencil(int NumDeps, int iterations) {
  fprintf(stderr, "Running benchmark with %i dependencies\n", NumDeps);
#pragma omp parallel
  {
    for (int i = 0; i < innerreps; i++) {
#pragma omp master
      {
        for (int iter = 0; iter < iterations; ++iter) {
          // swap input/output buffer for each iteration
          char * dep_in  = (iter % 2) ? buf1 : buf2;
          char * dep_out = (iter % 2) ? buf2 : buf1;
          for (int j = 0; j < numberoftasks; j++) {
            switch(NumDeps){
            case 0:
#pragma omp task
            { delay(delaylength); }
            break;
            case 1:
#pragma omp task depend(inout: dep_out[j])\
    depend(in: dep_in[j+1])
            { delay(delaylength); }
            break;
            case 2:
#pragma omp task depend(inout: dep_out[j])\
    depend(in: dep_in[j+1], dep_in[j-1] )
            { delay(delaylength); }
            break;
            case 4:
#pragma omp task depend(inout: dep_out[j])\
    depend(in:  dep_in[j+2], dep_in[j+1], dep_in[j-1], dep_in[j-2])
            { delay(delaylength); }
            break;
            case 8:
#pragma omp task depend(inout: dep_out[j])\
    depend(in:   dep_in[j+4], dep_in[j+3], dep_in[j+2], dep_in[j+1], dep_in[j-1], dep_in[j-2], dep_in[j-3], dep_in[j-4])
            { delay(delaylength); }
            break;
            case 16:
#pragma omp task depend(inout: dep_out[j])\
    depend(in:   dep_in[j+4], dep_in[j+3], dep_in[j+2], dep_in[j+1], dep_in[j-1], dep_in[j-2], dep_in[j-3], dep_in[j-4])\
    depend(in:   dep_in[j+8], dep_in[j+7], dep_in[j+6], dep_in[j+5], dep_in[j-5], dep_in[j-6], dep_in[j-7], dep_in[j-8])
            { delay(delaylength); }
            break;
            case 24:
#pragma omp task depend(inout: dep_out[j])\
    depend(in:   dep_in[j+4], dep_in[j+3], dep_in[j+2], dep_in[j+1], dep_in[j-1], dep_in[j-2], dep_in[j-3], dep_in[j-4])\
    depend(in:   dep_in[j+8], dep_in[j+7], dep_in[j+6], dep_in[j+5], dep_in[j-5], dep_in[j-6], dep_in[j-7], dep_in[j-8])\
    depend(in:   dep_in[j+12], dep_in[j+11], dep_in[j+10], dep_in[j+9], dep_in[j-9], dep_in[j-10], dep_in[j-11], dep_in[j-12])
            { delay(delaylength); }
            break;
            case 32:
#pragma omp task depend(inout: dep_out[j])\
    depend(in:   dep_in[j+4], dep_in[j+3], dep_in[j+2], dep_in[j+1], dep_in[j-1], dep_in[j-2], dep_in[j-3], dep_in[j-4])\
    depend(in:   dep_in[j+8], dep_in[j+7], dep_in[j+6], dep_in[j+5], dep_in[j-5], dep_in[j-6], dep_in[j-7], dep_in[j-8])\
    depend(in:   dep_in[j+12], dep_in[j+11], dep_in[j+10], dep_in[j+9], dep_in[j-9], dep_in[j-10], dep_in[j-11], dep_in[j-12])\
    depend(in:   dep_in[j+16], dep_in[j+15], dep_in[j+14], dep_in[j+13], dep_in[j-13], dep_in[j-14], dep_in[j-15], dep_in[j-16])
            { delay(delaylength); }
            break;
            }

          }
        }
      }
#pragma omp taskwait
    }
  }
}


void benchmark_stencil_samebuf(int NumDeps, int iterations) {
  fprintf(stderr, "Running benchmark with %i dependencies\n", NumDeps);
#pragma omp parallel
  {
    for (int i = 0; i < innerreps; i++) {
#pragma omp master
      {
//        printf("------------\n");
//        for (int iter = 0; iter < iterations; ++iter) {
        {
          for (int j = 0; j < numberoftasks*iterations; j++) {
            // swap input/output buffer for each iteration
            int dep_in =  ((j % TASK_GRAPH_WIDTH) + ((j / TASK_GRAPH_WIDTH) % 2 ? TASK_GRAPH_WIDTH : 0));
            int dep_out = ((j % TASK_GRAPH_WIDTH) + ((j / TASK_GRAPH_WIDTH) % 2 ? 0 : TASK_GRAPH_WIDTH));
            switch(NumDeps){
            case 0:
#pragma omp task
            { delay(delaylength); }
            break;
            case 1:
#pragma omp task depend(inout: depbuf[dep_out])\
    depend(in: depbuf[dep_in+1])
            { delay(delaylength); }
            break;
            case 2:
#pragma omp task depend(inout: depbuf[dep_out])\
    depend(in: depbuf[dep_in+1], depbuf[dep_in-1] )
            { delay(delaylength); }
            break;
            case 4:
//              printf("depend(in:  depbuf[%i], depbuf[%i], depbuf[%i], depbuf[%i]) dep_out:%i\n", dep_in+2, dep_in+1, dep_in-1, dep_in-2, dep_out);
#pragma omp task depend(inout: depbuf[dep_out])\
    depend(in:  depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in-1], depbuf[dep_in-2])
            { delay(delaylength); }
            break;
            case 8:
#pragma omp task depend(inout: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])
            { delay(delaylength); }
            break;
            case 16:
#pragma omp task depend(inout: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])
            { delay(delaylength); }
            break;
            case 24:
#pragma omp task depend(inout: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])\
    depend(in:   depbuf[dep_in+12], depbuf[dep_in+11], depbuf[dep_in+10], depbuf[dep_in+9], depbuf[dep_in-9], depbuf[dep_in-10], depbuf[dep_in-11], depbuf[dep_in-12])
            { delay(delaylength); }
            break;
            case 32:
#pragma omp task depend(inout: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])\
    depend(in:   depbuf[dep_in+12], depbuf[dep_in+11], depbuf[dep_in+10], depbuf[dep_in+9], depbuf[dep_in-9], depbuf[dep_in-10], depbuf[dep_in-11], depbuf[dep_in-12])\
    depend(in:   depbuf[dep_in+16], depbuf[dep_in+15], depbuf[dep_in+14], depbuf[dep_in+13], depbuf[dep_in-13], depbuf[dep_in-14], depbuf[dep_in-15], depbuf[dep_in-16])
            { delay(delaylength); }
            break;
            }

          }
        }
      }
#pragma omp taskwait
    }
  }
}


int main(int argc, char **argv) {

  // warm-up
  benchmark_stencil(32, 1);
  double delaytime = reference();

  fprintf(stderr, "\tdelaytime: %f us\n", delaytime);
  for (int n = 1; n <= 3; ++n) {
    fprintf(stderr, "Stencil dependencies (%i tasks)\n", numberoftasks * n);
    printf("NumDeps\tTime Per Task [us]\n");
    for (int i = 1; i <= MAX_DEPS; i *=2) {
      struct timespec start_ts, end_ts;
      clock_gettime(CLOCK_MONOTONIC, &start_ts);
      benchmark_stencil(i, n);
      clock_gettime(CLOCK_MONOTONIC, &end_ts);

      double us_per_task = (end_ts.tv_sec - start_ts.tv_sec) * 1E6
          + (end_ts.tv_nsec - start_ts.tv_nsec) / 1E3;
      us_per_task -= n * numberoftasks * delaytime;
      us_per_task /= n * numberoftasks;
      us_per_task /= innerreps;
      printf("%i\t%f\n", i, us_per_task);
    }
  }

  return EXIT_SUCCESS;

}

