/******
 *  COPYRIGHT
 *    (C) 2017 HLRS, University of Stuttgart
 *
 * Authors:
 *   Joseph Schuchart
 *   Mathias Nachtmann
 *
 ******/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "common.h"
#include "taskdepbench.h"

#define DEPTH 6

#define MAX_DEPS 32 // the maximum number of dependencies a task can have
int TASK_CHUNK_SIZE  =100; // the width of the task tree (number of parallel tasks)

#define TASK_GRAPH_WIDTH 1000

// pseudo buffer to map dependencies
static char *depbuf  = (char*)0xDEADBEEFL;

int main(int argc, char **argv) {

  init(argc, argv);

  //#ifdef OMPVER3
#if 1

  /* GENERATE REFERENCE TIME */
  reference("reference time 1", &refer);

#if 0

  /**
   * The following benchmarks are commented out because they do not
   * lead to meaningful results. However, they are left in as reference.
   */

  benchmark("TASK DEPENDENCY FANOUT 1", &testTaskDependencyFanOutMaster<1>);
  benchmark("TASK DEPENDENCY FANOUT 2", &testTaskDependencyFanOutMaster<2>);
  benchmark("TASK DEPENDENCY FANOUT 4", &testTaskDependencyFanOutMaster<4>);
  benchmark("TASK DEPENDENCY FANOUT 8", &testTaskDependencyFanOutMaster<8>);
  benchmark("TASK DEPENDENCY FANOUT 16", &testTaskDependencyFanOutMaster<16>);
  benchmark("TASK DEPENDENCY FANOUT 24", &testTaskDependencyFanOutMaster<24>);
  benchmark("TASK DEPENDENCY FANOUT 32", &testTaskDependencyFanOutMaster<32>);

  benchmark("TASK DEPENDENCY FANIN 1", &testTaskDependencyFanIn<1>);
  benchmark("TASK DEPENDENCY FANIN 2", &testTaskDependencyFanIn<2>);
  benchmark("TASK DEPENDENCY FANIN 4", &testTaskDependencyFanIn<4>);
  benchmark("TASK DEPENDENCY FANIN 8", &testTaskDependencyFanIn<8>);
  benchmark("TASK DEPENDENCY FANIN 16", &testTaskDependencyFanIn<16>);
  benchmark("TASK DEPENDENCY FANIN 24", &testTaskDependencyFanIn<24>);
  benchmark("TASK DEPENDENCY FANIN 32", &testTaskDependencyFanIn<32>);


  // benchmark("TASK DEPENDENCY TREE 1", &testTaskDependencyTree<1>);
  // benchmark("TASK DEPENDENCY TREE 2", &testTaskDependencyTree<2>);
  // benchmark("TASK DEPENDENCY TREE 4", &testTaskDependencyTree<4>);
  // benchmark("TASK DEPENDENCY TREE 8", &testTaskDependencyTree<8>);
  // benchmark("TASK DEPENDENCY TREE 16", &testTaskDependencyTree<16>);
  // benchmark("TASK DEPENDENCY TREE 24", &testTaskDependencyTree<24>);
  // benchmark("TASK DEPENDENCY TREE 32", &testTaskDependencyTree<32>);
  //
  //
  // benchmark("TASK DEPENDENCY REVERSEDTREE 1", &testTaskDependencyReversedTree<1>);
  // benchmark("TASK DEPENDENCY REVERSEDTREE 2", &testTaskDependencyReversedTree<2>);
  // benchmark("TASK DEPENDENCY REVERSEDTREE 4", &testTaskDependencyReversedTree<4>);
  // benchmark("TASK DEPENDENCY REVERSEDTREE 8", &testTaskDependencyReversedTree<8>);
  // benchmark("TASK DEPENDENCY REVERSEDTREE 16", &testTaskDependencyReversedTree<16>);
  // benchmark("TASK DEPENDENCY REVERSEDTREE 24", &testTaskDependencyReversedTree<24>);
  // benchmark("TASK DEPENDENCY REVERSEDTREE 32", &testTaskDependencyReversedTree<32>);
  benchmark("TASK DEPENDENCY NEIGHBORS 1", &testTaskDependencyNeighbors<1>);
  benchmark("TASK DEPENDENCY NEIGHBORS 2", &testTaskDependencyNeighbors<2>);
  benchmark("TASK DEPENDENCY NEIGHBORS 4", &testTaskDependencyNeighbors<4>);
  benchmark("TASK DEPENDENCY NEIGHBORS 8", &testTaskDependencyNeighbors<8>);
  benchmark("TASK DEPENDENCY NEIGHBORS 16", &testTaskDependencyNeighbors<16>);
  benchmark("TASK DEPENDENCY NEIGHBORS 24", &testTaskDependencyNeighbors<24>);
  benchmark("TASK DEPENDENCY NEIGHBORS 32", &testTaskDependencyNeighbors<32>);
  benchmark("TASK DEPENDENCY NEIGHBORS_STRIDE 1", &testTaskDependencyNeighbors<1,1000>);
  benchmark("TASK DEPENDENCY NEIGHBORS_STRIDE 2", &testTaskDependencyNeighbors<2,1000>);
  benchmark("TASK DEPENDENCY NEIGHBORS_STRIDE 4", &testTaskDependencyNeighbors<4,1000>);
  benchmark("TASK DEPENDENCY NEIGHBORS_STRIDE 8", &testTaskDependencyNeighbors<8,1000>);
  benchmark("TASK DEPENDENCY NEIGHBORS_STRIDE 16", &testTaskDependencyNeighbors<16,1000>);
  benchmark("TASK DEPENDENCY NEIGHBORS_STRIDE 24", &testTaskDependencyNeighbors<24,1000>);
  benchmark("TASK DEPENDENCY NEIGHBORS_STRIDE 32", &testTaskDependencyNeighbors<32,1000>);

  benchmark("TASK DEPENDENCY NEIGHBORSREV 1", &testTaskDependencyNeighborsReverse<1>);
  benchmark("TASK DEPENDENCY NEIGHBORSREV 2", &testTaskDependencyNeighborsReverse<2>);
  benchmark("TASK DEPENDENCY NEIGHBORSREV 4", &testTaskDependencyNeighborsReverse<4>);
  benchmark("TASK DEPENDENCY NEIGHBORSREV 8", &testTaskDependencyNeighborsReverse<8>);
  benchmark("TASK DEPENDENCY NEIGHBORSREV 16", &testTaskDependencyNeighborsReverse<16>);
  benchmark("TASK DEPENDENCY NEIGHBORSREV 24", &testTaskDependencyNeighborsReverse<24>);
  benchmark("TASK DEPENDENCY NEIGHBORSREV 32", &testTaskDependencyNeighborsReverse<32>);
#endif

  benchmark("TASK DEPENDENCY NEIGHBORS MASTER 1", &testTaskDependencyNeighborsMaster<1>);
  benchmark("TASK DEPENDENCY NEIGHBORS MASTER 2", &testTaskDependencyNeighborsMaster<2>);
  benchmark("TASK DEPENDENCY NEIGHBORS MASTER 4", &testTaskDependencyNeighborsMaster<4>);
  benchmark("TASK DEPENDENCY NEIGHBORS MASTER 8", &testTaskDependencyNeighborsMaster<8>);
  benchmark("TASK DEPENDENCY NEIGHBORS MASTER 16", &testTaskDependencyNeighborsMaster<16>);
  benchmark("TASK DEPENDENCY NEIGHBORS MASTER 24", &testTaskDependencyNeighborsMaster<24>);
  benchmark("TASK DEPENDENCY NEIGHBORS MASTER 32", &testTaskDependencyNeighborsMaster<32>);

  benchmark("TASK DEPENDENCY IN LINKED MASTER 1", &testTaskDependencyInLinkedMaster<1>);
  benchmark("TASK DEPENDENCY IN LINKED MASTER 2", &testTaskDependencyInLinkedMaster<2>);
  benchmark("TASK DEPENDENCY IN LINKED MASTER 4", &testTaskDependencyInLinkedMaster<4>);
  benchmark("TASK DEPENDENCY IN LINKED MASTER 8", &testTaskDependencyInLinkedMaster<8>);
  benchmark("TASK DEPENDENCY IN LINKED MASTER 16", &testTaskDependencyInLinkedMaster<16>);
  benchmark("TASK DEPENDENCY IN LINKED MASTER 24", &testTaskDependencyInLinkedMaster<24>);
  benchmark("TASK DEPENDENCY IN LINKED MASTER 32", &testTaskDependencyInLinkedMaster<32>);

  benchmark("TASK DEPENDENCY OUT LINKED MASTER 1", &testTaskDependencyOutLinkedMaster<1>);
  benchmark("TASK DEPENDENCY OUT LINKED MASTER 2", &testTaskDependencyOutLinkedMaster<2>);
  benchmark("TASK DEPENDENCY OUT LINKED MASTER 4", &testTaskDependencyOutLinkedMaster<4>);
  benchmark("TASK DEPENDENCY OUT LINKED MASTER 8", &testTaskDependencyOutLinkedMaster<8>);
  benchmark("TASK DEPENDENCY OUT LINKED MASTER 16", &testTaskDependencyOutLinkedMaster<16>);
  benchmark("TASK DEPENDENCY OUT LINKED MASTER 24", &testTaskDependencyOutLinkedMaster<24>);
  benchmark("TASK DEPENDENCY OUT LINKED MASTER 32", &testTaskDependencyOutLinkedMaster<32>);

  benchmark("TASK DEPENDENCY INOUT LINKED MASTER 1", &testTaskDependencyInOutLinkedMaster<1>);
  benchmark("TASK DEPENDENCY INOUT LINKED MASTER 2", &testTaskDependencyInOutLinkedMaster<2>);
  benchmark("TASK DEPENDENCY INOUT LINKED MASTER 4", &testTaskDependencyInOutLinkedMaster<4>);
  benchmark("TASK DEPENDENCY INOUT LINKED MASTER 8", &testTaskDependencyInOutLinkedMaster<8>);
  benchmark("TASK DEPENDENCY INOUT LINKED MASTER 16", &testTaskDependencyInOutLinkedMaster<16>);
  benchmark("TASK DEPENDENCY INOUT LINKED MASTER 24", &testTaskDependencyInOutLinkedMaster<24>);
  benchmark("TASK DEPENDENCY INOUT LINKED MASTER 32", &testTaskDependencyInOutLinkedMaster<32>);

  benchmark("TASK DEPENDENCY OUT CHAIN MASTER 1", &testTaskDependencyOutChainMaster<1>);
  benchmark("TASK DEPENDENCY OUT CHAIN MASTER 2", &testTaskDependencyOutChainMaster<2>);
  benchmark("TASK DEPENDENCY OUT CHAIN MASTER 4", &testTaskDependencyOutChainMaster<4>);
  benchmark("TASK DEPENDENCY OUT CHAIN MASTER 8", &testTaskDependencyOutChainMaster<8>);
  benchmark("TASK DEPENDENCY OUT CHAIN MASTER 16", &testTaskDependencyOutChainMaster<16>);
  benchmark("TASK DEPENDENCY OUT CHAIN MASTER 24", &testTaskDependencyOutChainMaster<24>);
  benchmark("TASK DEPENDENCY OUT CHAIN MASTER 32", &testTaskDependencyOutChainMaster<32>);

  benchmark("TASK DEPENDENCY INOUT CHAIN MASTER 1", &testTaskDependencyInOutChainMaster<1>);
  benchmark("TASK DEPENDENCY INOUT CHAIN MASTER 2", &testTaskDependencyInOutChainMaster<2>);
  benchmark("TASK DEPENDENCY INOUT CHAIN MASTER 4", &testTaskDependencyInOutChainMaster<4>);
  benchmark("TASK DEPENDENCY INOUT CHAIN MASTER 8", &testTaskDependencyInOutChainMaster<8>);
  benchmark("TASK DEPENDENCY INOUT CHAIN MASTER 16", &testTaskDependencyInOutChainMaster<16>);
  benchmark("TASK DEPENDENCY INOUT CHAIN MASTER 24", &testTaskDependencyInOutChainMaster<24>);
  benchmark("TASK DEPENDENCY INOUT CHAIN MASTER 32", &testTaskDependencyInOutChainMaster<32>);
#endif // OMPVER3

  finalise();

  return EXIT_SUCCESS;

}

/* Calculate the reference time. */
void refer() {
  int j;
  for (j = 0; j < innerreps; j++) {
    delay(delaylength);
  }

}


template<int NumDeps,int stride>
void testTaskDependencyNeighbors() {
  // thread 0 might create more tasks than the others
  // (to ensure constant number of tasks)
  int threadnum  = omp_get_thread_num();
  int numthreads = omp_get_num_threads();
  int max_tasks_per_thread = numberoftasks - ((numthreads - 1) * (numberoftasks / numthreads));
  int tasks_per_thread = (threadnum) ? numberoftasks / numthreads  : max_tasks_per_thread;

  for (int i = 0; i < innerreps; i++) {
    for (int j = 0; j < tasks_per_thread; j++) {
      // swap buffers every 1000 tasks (to create rows in the dependency graph
      //int dep_in =  (j + ((j / 1000) % 2 ? tasks_per_thread : 0)  + 2 * max_tasks_per_thread * threadnum) * stride;
      //int dep_out = (j + ((j / 1000) % 2 ? 0 : tasks_per_thread)  + 2 * max_tasks_per_thread * threadnum) * stride;
      // wrap-around task dependencies at the width of the task graph, i.e.,
      // dep_in=[0..1000], dep_out=[1000..2000] for the first 1000 tasks
      // dep_in=[1000..2000], dep_out=[0..1000] for the second 1000 tasks and so on
      int dep_in =  (((j % TASK_GRAPH_WIDTH) + 2*((j / TASK_GRAPH_WIDTH) % 2 ? TASK_GRAPH_WIDTH : 0)) + 2 * max_tasks_per_thread * threadnum) * stride;
      int dep_out = (((j % TASK_GRAPH_WIDTH) + 2*((j / TASK_GRAPH_WIDTH) % 2 ? 0 : TASK_GRAPH_WIDTH)) + 2 * max_tasks_per_thread * threadnum) * stride;
      //#pragma omp critical
      //      {printf("[%i] task:%i dep_in:%i dep_out:%i\n", threadnum, j, dep_in, dep_out);}
      switch(NumDeps){
      case 0:
#pragma omp task
      { delay(delaylength); }
      break;
      case 1:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in: depbuf[dep_in])
      { delay(delaylength); }
      break;
      case 2:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in: depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1] )
      { delay(delaylength); }
      break;
      case 4:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:  depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2])
      { delay(delaylength); }
      break;
      case 8:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])
      { delay(delaylength); }
      break;
      case 16:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])
      { delay(delaylength); }
      break;
      case 24:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])\
    depend(in:   depbuf[dep_in+12], depbuf[dep_in+11], depbuf[dep_in+10], depbuf[dep_in+9], depbuf[dep_in-9], depbuf[dep_in-10], depbuf[dep_in-11], depbuf[dep_in-12])
      { delay(delaylength); }
      break;
      case 32:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])\
    depend(in:   depbuf[dep_in+12], depbuf[dep_in+11], depbuf[dep_in+10], depbuf[dep_in+9], depbuf[dep_in-9], depbuf[dep_in-10], depbuf[dep_in-11], depbuf[dep_in-12])\
    depend(in:   depbuf[dep_in+16], depbuf[dep_in+15], depbuf[dep_in+14], depbuf[dep_in+13], depbuf[dep_in-13], depbuf[dep_in-14], depbuf[dep_in-15], depbuf[dep_in-16])
      { delay(delaylength); }
      break;
      }

    }
#pragma omp taskwait
  }
}



template<int NumDeps>
void testTaskDependencyNeighborsReverse() {
  // thread 0 might have to create more tasks than the others
  // (to ensure constant number of tasks)
  int threadnum  = omp_get_thread_num();
  int numthreads = omp_get_num_threads();
  int max_tasks_per_thread = numberoftasks - ((numthreads - 1) * (numberoftasks / numthreads));
  int tasks_per_thread = (threadnum) ? numberoftasks / numthreads
      : max_tasks_per_thread;
  // with OUT dependencies we need to go wider to avoid dependencies on one level
  const int width = TASK_GRAPH_WIDTH * NumDeps;
  for (int i = 0; i < innerreps; i++) {
    for (int j = 0; j < tasks_per_thread; j++) {
      //int dep = j*NumDeps/2 + (threadnum*max_tasks_per_thread);
      int dep_in =  ((((j * NumDeps) % width) +                       // Initial position with wrap-around
          ((j / TASK_GRAPH_WIDTH) % 2 ? width : 0))   // determine one of two buffers
          + 2 * width * threadnum);                   // offset to avoid overlap between threads
      int dep_out = ((((j * NumDeps) % width) + ((j / TASK_GRAPH_WIDTH) % 2 ? 0 : width)) + 2 * width * threadnum);
      //#pragma omp critical
      //      {printf("[%i] task:%i dep_in:%i dep_out:%i\n", threadnum, j, dep_in, dep_out);}
      switch(NumDeps){
      case 0:
#pragma omp task
      { delay(delaylength); }
      break;
      case 1:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out: depbuf[dep_out+1])
      { delay(delaylength); }
      break;
      case 2:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out: depbuf[dep_out+1], depbuf[dep_out-1] )
      { delay(delaylength); }
      break;
      case 4:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:  depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2])
      { delay(delaylength); }
      break;
      case 8:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:   depbuf[dep_out+4], depbuf[dep_out+3], depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2], depbuf[dep_out-3], depbuf[dep_out-4])
      { delay(delaylength); }
      break;
      case 16:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:   depbuf[dep_out+4], depbuf[dep_out+3], depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2], depbuf[dep_out-3], depbuf[dep_out-4])\
    depend(out:   depbuf[dep_out+8], depbuf[dep_out+7], depbuf[dep_out+6], depbuf[dep_out+5], depbuf[dep_out-5], depbuf[dep_out-6], depbuf[dep_out-7], depbuf[dep_out-8])
      { delay(delaylength); }
      break;
      case 24:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:   depbuf[dep_out+4], depbuf[dep_out+3], depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2], depbuf[dep_out-3], depbuf[dep_out-4])\
    depend(out:   depbuf[dep_out+8], depbuf[dep_out+7], depbuf[dep_out+6], depbuf[dep_out+5], depbuf[dep_out-5], depbuf[dep_out-6], depbuf[dep_out-7], depbuf[dep_out-8])\
    depend(out:   depbuf[dep_out+12], depbuf[dep_out+11], depbuf[dep_out+10], depbuf[dep_out+9], depbuf[dep_out-9], depbuf[dep_out-10], depbuf[dep_out-11], depbuf[dep_out-12])
      { delay(delaylength); }
      break;
      case 32:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:   depbuf[dep_out+4], depbuf[dep_out+3], depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2], depbuf[dep_out-3], depbuf[dep_out-4])\
    depend(out:   depbuf[dep_out+8], depbuf[dep_out+7], depbuf[dep_out+6], depbuf[dep_out+5], depbuf[dep_out-5], depbuf[dep_out-6], depbuf[dep_out-7], depbuf[dep_out-8])\
    depend(out:   depbuf[dep_out+12], depbuf[dep_out+11], depbuf[dep_out+10], depbuf[dep_out+9], depbuf[dep_out-9], depbuf[dep_out-10], depbuf[dep_out-11], depbuf[dep_out-12])\
    depend(out:   depbuf[dep_out+16], depbuf[dep_out+15], depbuf[dep_out+14], depbuf[dep_out+13], depbuf[dep_out-13], depbuf[dep_out-14], depbuf[dep_out-15], depbuf[dep_out-16])
      { delay(delaylength); }
      break;
      }
    }
#pragma omp taskwait
  }
}


template<int NumDeps,int stride>
void testTaskDependencyNeighborsMaster() {
  // thread 0 might create more tasks than the others
  // (to ensure constant number of tasks)
  for (int i = 0; i < innerreps; i++) {
#pragma omp master
    {
      for (int j = 0; j < numberoftasks; j++) {
        // wrap-around task dependencies at the width of the task graph, i.e.,
        // dep_in=[0..1000], dep_out=[1000..2000] for the first 1000 tasks
        int dep_in =  ((j % TASK_GRAPH_WIDTH) + 2*((j / TASK_GRAPH_WIDTH) % 2 ? TASK_GRAPH_WIDTH : 0));
        int dep_out = ((j % TASK_GRAPH_WIDTH) + 2*((j / TASK_GRAPH_WIDTH) % 2 ? 0 : TASK_GRAPH_WIDTH));
        //printf("task:%i dep_in:%i dep_out:%i\n", j, dep_in, dep_out);
        switch(NumDeps){
        case 0:
#pragma omp task depend(out: depbuf[dep_out]) depend(in: depbuf[dep_in])
          { delay(delaylength); }
          break;
        case 1:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in: depbuf[dep_in], depbuf[dep_in+1])
          { delay(delaylength); }
          break;
        case 2:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in: depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1] )
        { delay(delaylength); }
        break;
        case 4:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:  depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2])
          { delay(delaylength); }
          break;
        case 8:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])
          { delay(delaylength); }
          break;
        case 16:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])
          { delay(delaylength); }
          break;
        case 24:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])\
    depend(in:   depbuf[dep_in+12], depbuf[dep_in+11], depbuf[dep_in+10], depbuf[dep_in+9], depbuf[dep_in-9], depbuf[dep_in-10], depbuf[dep_in-11], depbuf[dep_in-12])
          { delay(delaylength); }
          break;
        case 32:
#pragma omp task depend(out: depbuf[dep_out])\
    depend(in:   depbuf[dep_in+4], depbuf[dep_in+3], depbuf[dep_in+2], depbuf[dep_in+1], depbuf[dep_in], depbuf[dep_in-1], depbuf[dep_in-2], depbuf[dep_in-3], depbuf[dep_in-4])\
    depend(in:   depbuf[dep_in+8], depbuf[dep_in+7], depbuf[dep_in+6], depbuf[dep_in+5], depbuf[dep_in-5], depbuf[dep_in-6], depbuf[dep_in-7], depbuf[dep_in-8])\
    depend(in:   depbuf[dep_in+12], depbuf[dep_in+11], depbuf[dep_in+10], depbuf[dep_in+9], depbuf[dep_in-9], depbuf[dep_in-10], depbuf[dep_in-11], depbuf[dep_in-12])\
    depend(in:   depbuf[dep_in+16], depbuf[dep_in+15], depbuf[dep_in+14], depbuf[dep_in+13], depbuf[dep_in-13], depbuf[dep_in-14], depbuf[dep_in-15], depbuf[dep_in-16])
          { delay(delaylength); }
          break;
        }
      }
#pragma omp taskwait
    }
  }
}



template<int NumDeps>
void testTaskDependencyNeighborsReverseMaster() {
  for (int i = 0; i < innerreps; i++) {
#pragma omp master
    {
      for (int j = 0; j < numberoftasks; j++) {
        //int dep = j* NumDeps/2;
        int width = TASK_GRAPH_WIDTH * NumDeps;
        int dep_in =  ((((j * NumDeps) % width) +         // Initial position with wrap-around
            ((j / TASK_GRAPH_WIDTH) % 2 ? width : 0)));   // determine one of two buffers
        int dep_out = ((((j * NumDeps) % width) + ((j / TASK_GRAPH_WIDTH) % 2 ? 0 : width)));

        switch(NumDeps){
        case 0:
#pragma omp task
          { delay(delaylength); }
          break;
        case 1:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out: depbuf[dep_out+1])
          { delay(delaylength); }
          break;
        case 2:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out: depbuf[dep_out+1], depbuf[dep_out-1] )
          { delay(delaylength); }
          break;
        case 4:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:  depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2])
          { delay(delaylength); }
          break;
        case 8:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:   depbuf[dep_out+4], depbuf[dep_out+3], depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2], depbuf[dep_out-3], depbuf[dep_out-4])
          { delay(delaylength); }
          break;
        case 16:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:   depbuf[dep_out+4], depbuf[dep_out+3], depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2], depbuf[dep_out-3], depbuf[dep_out-4])\
    depend(out:   depbuf[dep_out+8], depbuf[dep_out+7], depbuf[dep_out+6], depbuf[dep_out+5], depbuf[dep_out-5], depbuf[dep_out-6], depbuf[dep_out-7], depbuf[dep_out-8])
          { delay(delaylength); }
          break;
        case 24:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:   depbuf[dep_out+4], depbuf[dep_out+3], depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2], depbuf[dep_out-3], depbuf[dep_out-4])\
    depend(out:   depbuf[dep_out+8], depbuf[dep_out+7], depbuf[dep_out+6], depbuf[dep_out+5], depbuf[dep_out-5], depbuf[dep_out-6], depbuf[dep_out-7], depbuf[dep_out-8])\
    depend(out:   depbuf[dep_out+12], depbuf[dep_out+11], depbuf[dep_out+10], depbuf[dep_out+9], depbuf[dep_out-9], depbuf[dep_out-10], depbuf[dep_out-11], depbuf[dep_out-12])
          { delay(delaylength); }
          break;
        case 32:
#pragma omp task depend(inout: depbuf[dep_in])\
    depend(out:   depbuf[dep_out+4], depbuf[dep_out+3], depbuf[dep_out+2], depbuf[dep_out+1], depbuf[dep_out-1], depbuf[dep_out-2], depbuf[dep_out-3], depbuf[dep_out-4])\
    depend(out:   depbuf[dep_out+8], depbuf[dep_out+7], depbuf[dep_out+6], depbuf[dep_out+5], depbuf[dep_out-5], depbuf[dep_out-6], depbuf[dep_out-7], depbuf[dep_out-8])\
    depend(out:   depbuf[dep_out+12], depbuf[dep_out+11], depbuf[dep_out+10], depbuf[dep_out+9], depbuf[dep_out-9], depbuf[dep_out-10], depbuf[dep_out-11], depbuf[dep_out-12])\
    depend(out:   depbuf[dep_out+16], depbuf[dep_out+15], depbuf[dep_out+14], depbuf[dep_out+13], depbuf[dep_out-13], depbuf[dep_out-14], depbuf[dep_out-15], depbuf[dep_out-16])
          { delay(delaylength); }
          break;
        }
      }
#pragma omp taskwait
    }
  }
}



template<int NumDeps>
void testTaskDependencyOutChainMaster() {
  for (int i = 0; i < innerreps; i++) {
#pragma omp master
    {
      for (int j = 0; j < numberoftasks; j++) {
        int dep = 0; // in the chain, all tasks are connected along one line
        switch(NumDeps){
        case 0:
#pragma omp task
          { delay(delaylength); }
          break;
        case 1:
#pragma omp task depend(out: depbuf[dep + 0]) 
          { delay(delaylength); }
          break;
        case 2:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1]) 
          { delay(delaylength); }
          break;
        case 4:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3]) 
          { delay(delaylength); }
          break;
        case 8:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) 
          { delay(delaylength); }
          break;
        case 16:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) \
    depend(out: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])
          { delay(delaylength); }
          break;
        case 24:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(out: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(out: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])
          { delay(delaylength); }
          break;
        case 32:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(out: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(out: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])\
    depend(out: depbuf[dep + 24], depbuf[dep + 25], depbuf[dep + 26], depbuf[dep + 27], depbuf[dep + 28], depbuf[dep + 29], depbuf[dep + 30], depbuf[dep + 31])
          { delay(delaylength); }
          break;
        }
      }
#pragma omp taskwait
    }
  }
}

template<int NumDeps>
void testTaskDependencyInChainMaster() {
  for (int i = 0; i < innerreps; i++) {
#pragma omp master
    {
      for (int j = 0; j < numberoftasks; j++) {
        int dep = 0;
        switch(NumDeps){
        case 0:
#pragma omp task
          { delay(delaylength); }
          break;
        case 1:
#pragma omp task depend(inout: depbuf[dep + 0]) 
          { delay(delaylength); }
          break;
        case 2:
#pragma omp task depend(in: depbuf[dep + 0]) depend(inout: depbuf[dep + 1]) 
          { delay(delaylength); }
          break;
        case 4:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2]) depend(inout: depbuf[dep + 3]) 
          { delay(delaylength); }
          break;
        case 8:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6]) \
    depend(inout: depbuf[dep + 7])
          { delay(delaylength); }
          break;
        case 16:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) \
    depend(in: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14]) \
    depend(inout: depbuf[dep + 15])
          { delay(delaylength); }
          break;
        case 24:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(in: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(in: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22]) \
    depend(inout: depbuf[dep + 23])
          { delay(delaylength); }
          break;
        case 32:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(in: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(in: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])\
    depend(in: depbuf[dep + 24], depbuf[dep + 25], depbuf[dep + 26], depbuf[dep + 27], depbuf[dep + 28], depbuf[dep + 29], depbuf[dep + 30]) \
    depend(inout: depbuf[dep + 31])
          { delay(delaylength); }
          break;
        }
      }
#pragma omp taskwait
    }
  }
}

template<int NumDeps>
void testTaskDependencyInOutChainMaster() {
  for (int i = 0; i < innerreps; i++) {
#pragma omp master
    {
      for (int j = 0; j < numberoftasks; j++) {
        int dep = 0;
        switch(NumDeps){
        case 0:
#pragma omp task
          { delay(delaylength); }
          break;
        case 1:
#pragma omp task depend(inout: depbuf[dep + 0]) 
          { delay(delaylength); }
          break;
        case 2:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1]) 
          { delay(delaylength); }
          break;
        case 4:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3]) 
          { delay(delaylength); }
          break;
        case 8:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) 
          { delay(delaylength); }
          break;
        case 16:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) \
    depend(inout: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])
          { delay(delaylength); }
          break;
        case 24:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(inout: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(inout: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])
          { delay(delaylength); }
          break;
        case 32:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(inout: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(inout: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])\
    depend(inout: depbuf[dep + 24], depbuf[dep + 25], depbuf[dep + 26], depbuf[dep + 27], depbuf[dep + 28], depbuf[dep + 29], depbuf[dep + 30], depbuf[dep + 31])
          { delay(delaylength); }
          break;
        }
      }
#pragma omp taskwait
    }
  }
}

template<int NumDeps>
void testTaskDependencyOutLinkedMaster() {
  for (int i = 0; i < innerreps; i++) {
#pragma omp master
    {
      for (int j = 0; j < numberoftasks; j++) {
        int dep = j*(NumDeps - 1);
        switch(NumDeps){
        case 0:
#pragma omp task
          { delay(delaylength); }
          break;
        case 1:
#pragma omp task depend(out: depbuf[dep + 0]) 
          { delay(delaylength); }
          break;
        case 2:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1]) 
          { delay(delaylength); }
          break;
        case 4:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3]) 
          { delay(delaylength); }
          break;
        case 8:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) 
          { delay(delaylength); }
          break;
        case 16:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) \
    depend(out: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])
          { delay(delaylength); }
          break;
        case 24:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(out: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(out: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])
          { delay(delaylength); }
          break;
        case 32:
#pragma omp task depend(out: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(out: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(out: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])\
    depend(out: depbuf[dep + 24], depbuf[dep + 25], depbuf[dep + 26], depbuf[dep + 27], depbuf[dep + 28], depbuf[dep + 29], depbuf[dep + 30], depbuf[dep + 31])
          { delay(delaylength); }
          break;
        }
      }
#pragma omp taskwait
    }
  }
}

template<int NumDeps>
void testTaskDependencyInLinkedMaster() {
  for (int i = 0; i < innerreps; i++) {
#pragma omp master
    {
      for (int j = 0; j < numberoftasks; j++) {
        int dep = j*(NumDeps - 1);
        switch(NumDeps){
        case 0:
#pragma omp task
          { delay(delaylength); }
          break;
        case 1:
#pragma omp task depend(inout: depbuf[dep + 0]) 
          { delay(delaylength); }
          break;
        case 2:
#pragma omp task depend(in: depbuf[dep + 0]) depend(inout: depbuf[dep + 1]) 
          { delay(delaylength); }
          break;
        case 4:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2]) depend(inout: depbuf[dep + 3]) 
          { delay(delaylength); }
          break;
        case 8:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6]) \
    depend(inout: depbuf[dep + 7])
          { delay(delaylength); }
          break;
        case 16:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) \
    depend(in: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14]) \
    depend(inout: depbuf[dep + 15])
          { delay(delaylength); }
          break;
        case 24:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(in: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(in: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22]) \
    depend(inout: depbuf[dep + 23])
          { delay(delaylength); }
          break;
        case 32:
#pragma omp task depend(in: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(in: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(in: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])\
    depend(in: depbuf[dep + 24], depbuf[dep + 25], depbuf[dep + 26], depbuf[dep + 27], depbuf[dep + 28], depbuf[dep + 29], depbuf[dep + 30]) \
    depend(inout: depbuf[dep + 31])
          { delay(delaylength); }
          break;
        }
      }
#pragma omp taskwait
    }
  }
}



template<int NumDeps>
void testTaskDependencyInOutLinkedMaster() {
  for (int i = 0; i < innerreps; i++) {
#pragma omp master
    {
      for (int j = 0; j < numberoftasks; j++) {
        int dep = j*(NumDeps - 1);
        switch(NumDeps){
        case 0:
#pragma omp task
          { delay(delaylength); }
          break;
        case 1:
#pragma omp task depend(inout: depbuf[dep + 0]) 
          { delay(delaylength); }
          break;
        case 2:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1]) 
          { delay(delaylength); }
          break;
        case 4:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3]) 
          { delay(delaylength); }
          break;
        case 8:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) 
          { delay(delaylength); }
          break;
        case 16:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7]) \
    depend(inout: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])
          { delay(delaylength); }
          break;
        case 24:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(inout: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(inout: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])
        { delay(delaylength); }
        break;
        case 32:
#pragma omp task depend(inout: depbuf[dep + 0], depbuf[dep + 1], depbuf[dep + 2], depbuf[dep + 3], depbuf[dep + 4], depbuf[dep + 5], depbuf[dep + 6], depbuf[dep + 7])\
    depend(inout: depbuf[dep + 8], depbuf[dep + 9], depbuf[dep + 10], depbuf[dep + 11], depbuf[dep + 12], depbuf[dep + 13], depbuf[dep + 14], depbuf[dep + 15])\
    depend(inout: depbuf[dep + 16], depbuf[dep + 17], depbuf[dep + 18], depbuf[dep + 19], depbuf[dep + 20], depbuf[dep + 21], depbuf[dep + 22], depbuf[dep + 23])\
    depend(inout: depbuf[dep + 24], depbuf[dep + 25], depbuf[dep + 26], depbuf[dep + 27], depbuf[dep + 28], depbuf[dep + 29], depbuf[dep + 30], depbuf[dep + 31])
          { delay(delaylength); }
          break;
        }
      }
#pragma omp taskwait
    }
  }
}
