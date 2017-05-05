/****************************************************************************
*                                                                           *
*             OpenMP MicroBenchmark Suite - Version 3.1                     *
*                                                                           *
*                            produced by                                    *
*                                                                           *
*             Mark Bull, Fiona Reid and Nix Mc Donnell                      *
*                                                                           *
*                                at                                         *
*                                                                           *
*                Edinburgh Parallel Computing Centre                        *
*                                                                           *
*         email: markb@epcc.ed.ac.uk or fiona@epcc.ed.ac.uk                 *
*                                                                           *
*                                                                           *
*      This version copyright (c) The University of Edinburgh, 2015.        *
*                                                                           *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*      http://www.apache.org/licenses/LICENSE-2.0                           *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "common.h"
#include "taskbench.h"

#define DEPTH 6

#define MAX_DEPS 27 // the maximum number of dependencies a task can have
#define TASK_CHUNK_SIZE 100 // the width of the task tree (number of parallel tasks)

static char *depbuf = NULL;

static double *l2buf = NULL;
static size_t l2size = 0;

static uint32_t i386_cpuid_cachesize(int level);

int main(int argc, char **argv) {

    init(argc, argv);

    depbuf = new char[TASK_CHUNK_SIZE * MAX_DEPS];
    l2size = i386_cpuid_cachesize(2) / sizeof(double);
    l2buf = new double[l2size];

#ifdef OMPVER3

    /* GENERATE REFERENCE TIME */
    reference("reference time 1", &refer);

    /* TEST PARALLEL TASK GENERATION */
    benchmark("PARALLEL TASK", &testParallelTaskGeneration);

    /* TEST MASTER TASK GENERATION */
    benchmark("MASTER TASK", &testMasterTaskGeneration);

    /* TEST MASTER TASK GENERATION WITH BUSY SLAVES */
    benchmark("MASTER TASK BUSY SLAVES", &testMasterTaskGenerationWithBusySlaves);

    /* TEST CONDITIONAL TASK GENERATION */
#ifndef DISABLE_CONDITIONAL_TASK_TEST
    benchmark("CONDITIONAL TASK", &testConditionalTaskGeneration);
#endif // DISABLE_CONDITIONAL_TASK_TEST

    /* TEST TASK WAIT */
    benchmark("TASK WAIT", &testTaskWait);

    /* TEST TASK BARRIER */
#ifndef DISABLE_BARRIER_TEST
    benchmark("TASK BARRIER", &testTaskBarrier);
#endif //DISABLE_BARRIER_TEST

#ifndef DISABLE_NESTED_TASKS_TESTS
    /* TEST NESTED TASK GENERATION */
    benchmark("NESTED TASK", &testNestedTaskGeneration);

    /* TEST NESTED MASTER TASK GENERATION */
    benchmark("NESTED MASTER TASK", &testNestedMasterTaskGeneration);

#endif // DISABLE_NESTED_TASKS_TESTS
#if 0
    /* GENERATE THE SECOND REFERENCE TIME */
    reference("reference time 2", &refer);

    /* TEST BRANCH TASK TREE */
    benchmark("BRANCH TASK TREE", &testBranchTaskGeneration);

    /* TEST LEAF TASK TREE */
    benchmark("LEAF TASK TREE", &testLeafTaskGeneration);
#endif 

    benchmark("TASK DEPENDENCY MASTER 0", &testTaskDependencyMaster<0>);
    benchmark("TASK DEPENDENCY MASTER 1", &testTaskDependencyMaster<1>);
    benchmark("TASK DEPENDENCY MASTER 2", &testTaskDependencyMaster<2>);
    benchmark("TASK DEPENDENCY MASTER 4", &testTaskDependencyMaster<4>);
    benchmark("TASK DEPENDENCY MASTER 8", &testTaskDependencyMaster<8>);
    benchmark("TASK DEPENDENCY MASTER 18", &testTaskDependencyMaster<18>);
    benchmark("TASK DEPENDENCY MASTER 27", &testTaskDependencyMaster<27>);

#endif // OMPVER3

    finalise();

    delete[] depbuf;
    delete[] l2buf;

    return EXIT_SUCCESS;
}

/* Calculate the reference time. */
void refer() {
    int j;
    for (j = 0; j < innerreps; j++) {
	delay_l2clear(delaylength);
    }

}

/* Calculate the second reference time. */
void refer2() {
    int j;
    for (j = 0; j < (innerreps >> DEPTH) * (1 << DEPTH); j++) {
	delay_l2clear(delaylength);
    };

}

/* Test parallel task generation overhead */
void testParallelTaskGeneration() {
    int j;
//#pragma omp parallel private( j )
    {
	for ( j = 0; j < innerreps; j ++ ) {
#pragma omp task
	    {
		delay_l2clear( delaylength );

	    } // task
	}; // for j
    } // parallel

}

/* Test master task generation overhead */
void testMasterTaskGeneration() {
    int j;
//#pragma omp parallel private(j)
    {
#pragma omp master
	{
	    /* Since this is executed by one thread we need innerreps * nthreads
	       iterations */
	    for (j = 0; j < innerreps * nthreads; j++) {
#pragma omp task
		{
		    delay_l2clear(delaylength);

		}

	    } /* End for j */
	} /* End master */
    } /* End parallel */

}

/* Test master task generation overhead when the slave threads are busy */
void testMasterTaskGenerationWithBusySlaves() {
    int j;
//#pragma omp parallel private( j )
    {
	int thread_num = omp_get_thread_num();
	for (j = 0; j < innerreps; j ++ ) {

	    if ( thread_num == 0 ) {
#pragma omp task
		{
		    delay_l2clear( delaylength );
		} // task

	    } else {
		delay_l2clear( delaylength );

	    }; // if
	}; // for j
    } // parallel
}

/* Measure overhead of checking if a task should be spawned. */
void testConditionalTaskGeneration() {
    int j;
//#pragma omp parallel private(j)
    {
	for (j = 0; j < innerreps; j++) {
#pragma omp task if(returnfalse())
	    {
		delay_l2clear( delaylength );
	    }
	}
    }
}

#ifndef DISABLE_NESTED_TASKS_TESTS

/* Measure overhead of nested tasks (all threads construct outer tasks) */
void testNestedTaskGeneration() {
    int i,j;
//#pragma omp parallel private( i, j )
    {
	for ( j = 0; j < innerreps / nthreads; j ++ ) {
#pragma omp task private( i )
	    {
		for ( i = 0; i < nthreads; i ++ ) {
#pragma omp task untied
		    {
			delay_l2clear( delaylength );

		    } // task
		}; // for i

		// wait for inner tasks to complete
#pragma omp taskwait

	    } // task
	}; // for j
    } // parallel
}

/* Measure overhead of nested tasks (master thread constructs outer tasks) */
void testNestedMasterTaskGeneration() {
    int i, j;
//#pragma omp parallel private( i, j )
    {
#pragma omp master
	{
	    for ( j = 0; j < innerreps; j ++ ) {
#pragma omp task private( i )
		{
		    for ( i = 0; i < nthreads; i ++ ) {
#pragma omp task
			{
			    delay_l2clear( delaylength );

			} // task
		    }; // for i

		    // wait for inner tasks to complete
#pragma omp taskwait

		} // task
	    }; // for j
	} // master
    } // parallel
}
#endif // DISABLE_NESTED_TASKS_TESTS

/* Measure overhead of taskwait (all threads construct tasks) */
void testTaskWait() {
    int j;
//#pragma omp parallel private( j )
    {
	for ( j = 0; j < innerreps; j ++ ) {
#pragma omp task
	    {
		delay_l2clear( delaylength );

	    } // task
#pragma omp taskwait

	}; // for j
    } // parallel
}

/* Measure overhead of tasking barrier (all threads construct tasks) */
void testTaskBarrier() {
    int j;
//#pragma omp parallel private( j )
    {
	for ( j = 0; j < innerreps; j ++ ) {
#pragma omp task
	    {
		delay_l2clear( delaylength );

	    } // task
#pragma omp barrier

	}; // for j
    } // parallel
}

/* Test parallel task generation overhead where work is done at all levels. */
void testBranchTaskGeneration() {
    int j;
//#pragma omp parallel private(j)
    {
	for (j = 0; j < (innerreps >> DEPTH); j++) {
#pragma omp task
	    {
		branchTaskTree(DEPTH);
		delay_l2clear(delaylength);
	    }

	}
    }
}

void branchTaskTree(int tree_level) {
    if ( tree_level > 0 ) {
#pragma omp task
	{
	    branchTaskTree(tree_level - 1);
	    branchTaskTree(tree_level - 1);
	    delay_l2clear(delaylength);
	}
    }
}

/* Test parallel task generation overhead where work is done only at the leaf level. */
void testLeafTaskGeneration() {
    int j;
//#pragma omp parallel private(j)
    {
	for (j = 0; j < (innerreps >> DEPTH); j++) {
	    leafTaskTree(DEPTH);

	}
    }

}

void leafTaskTree(int tree_level) {
    if ( tree_level == 0 ) {
	delay_l2clear(delaylength);

    } else {
#pragma omp task
	{
	    leafTaskTree(tree_level - 1);
	    leafTaskTree(tree_level - 1);
	}
    }
}

template<int NumDeps>
void testTaskDependencyMaster() {

#pragma omp master
            {
                for (int i = 0; i < innerreps; i++) {
                    int dep = innerreps % TASK_CHUNK_SIZE;
                    switch(NumDeps){
                        case 0:
#pragma omp task
			    { delay_l2clear(delaylength); }
                            break;
                        case 1:
#pragma omp task depend(inout:depbuf[dep])
			    { delay_l2clear(delaylength); }
                            break;
                        case 2:
#pragma omp task depend(inout:depbuf[dep], depbuf[dep + TASK_CHUNK_SIZE])
			    { delay_l2clear(delaylength); }
                            break;
                        case 4:
#pragma omp task depend(inout:depbuf[dep], depbuf[dep + TASK_CHUNK_SIZE], depbuf[dep + 2*TASK_CHUNK_SIZE], depbuf[dep + 3*TASK_CHUNK_SIZE])
			    { delay_l2clear(delaylength); }
                            break;
                        case 8:
#pragma omp task depend(inout:depbuf[dep], depbuf[dep + TASK_CHUNK_SIZE], depbuf[dep + 2*TASK_CHUNK_SIZE], depbuf[dep + 3*TASK_CHUNK_SIZE], depbuf[dep + 4*TASK_CHUNK_SIZE], depbuf[dep + 5*TASK_CHUNK_SIZE], depbuf[dep + 6*TASK_CHUNK_SIZE], depbuf[dep + 7*TASK_CHUNK_SIZE])
			    { delay_l2clear(delaylength); }
                            break;
                        case 18:
#pragma omp task depend(inout: depbuf[dep]                    , depbuf[dep + 1*TASK_CHUNK_SIZE], depbuf[dep + 2*TASK_CHUNK_SIZE], depbuf[dep + 3*TASK_CHUNK_SIZE], depbuf[dep + 4*TASK_CHUNK_SIZE], depbuf[dep + 5*TASK_CHUNK_SIZE], depbuf[dep + 6*TASK_CHUNK_SIZE], depbuf[dep + 7*TASK_CHUNK_SIZE], depbuf[dep + 8*TASK_CHUNK_SIZE], depbuf[dep + 9*TASK_CHUNK_SIZE], depbuf[dep + 10*TASK_CHUNK_SIZE], depbuf[dep + 11*TASK_CHUNK_SIZE], depbuf[dep + 12*TASK_CHUNK_SIZE], depbuf[dep + 13*TASK_CHUNK_SIZE], depbuf[dep + 14*TASK_CHUNK_SIZE], depbuf[dep + 15*TASK_CHUNK_SIZE], depbuf[dep + 16*TASK_CHUNK_SIZE], depbuf[dep + 17*TASK_CHUNK_SIZE])
			    { delay_l2cleardelaylength); }
                            break;
                        case 27:
#pragma omp task depend(inout: depbuf[dep]                    ,  depbuf[dep + 1*TASK_CHUNK_SIZE],  depbuf[dep + 2*TASK_CHUNK_SIZE],  depbuf[dep + 3*TASK_CHUNK_SIZE])  \
                 depend(inout: depbuf[dep + 4*TASK_CHUNK_SIZE],  depbuf[dep + 5*TASK_CHUNK_SIZE],  depbuf[dep + 6*TASK_CHUNK_SIZE],  depbuf[dep + 7*TASK_CHUNK_SIZE])  \
                 depend(inout: depbuf[dep + 8*TASK_CHUNK_SIZE],  depbuf[dep + 9*TASK_CHUNK_SIZE],  depbuf[dep + 10*TASK_CHUNK_SIZE], depbuf[dep + 11*TASK_CHUNK_SIZE]) \
                 depend(inout: depbuf[dep + 12*TASK_CHUNK_SIZE], depbuf[dep + 13*TASK_CHUNK_SIZE], depbuf[dep + 14*TASK_CHUNK_SIZE], depbuf[dep + 15*TASK_CHUNK_SIZE]) \
                 depend(inout: depbuf[dep + 16*TASK_CHUNK_SIZE], depbuf[dep + 17*TASK_CHUNK_SIZE], depbuf[dep + 18*TASK_CHUNK_SIZE], depbuf[dep + 19*TASK_CHUNK_SIZE]) \
                 depend(inout: depbuf[dep + 20*TASK_CHUNK_SIZE], depbuf[dep + 21*TASK_CHUNK_SIZE], depbuf[dep + 22*TASK_CHUNK_SIZE], depbuf[dep + 23*TASK_CHUNK_SIZE]) \
                 depend(inout: depbuf[dep + 24*TASK_CHUNK_SIZE], depbuf[dep + 25*TASK_CHUNK_SIZE], depbuf[dep + 26*TASK_CHUNK_SIZE])
			    { delay_l2clear(delaylength); }
                            break;

                    }
                    delay_l2clear(delaylength);
                }
            }
}

void delay_l2clear(int delaylength) {
    double sum = 0.0;
    for (int i = 0; i < l2size; ++i) {
	sum += l2buf[i];
    }
    l2buf[0] = sum;
}


/** 
 * Query the size of the cache at the given level.
 *
 * Taken from http://stackoverflow.com/a/12697439
 */ 
static uint32_t i386_cpuid_cachesize(int level) {
    int i;
    for (i = 0; i < 32; i++) {

        // Variables to hold the contents of the 4 i386 legacy registers
        uint32_t eax, ebx, ecx, edx; 

        eax = 4; // get cache info
        ecx = i; // cache id

        __asm__ (
            "cpuid" // call i386 cpuid instruction
            : "+a" (eax) // contains the cpuid command code, 4 for cache query
            , "=b" (ebx)
            , "+c" (ecx) // contains the cache id
            , "=d" (edx)
        ); // generates output in 4 registers eax, ebx, ecx and edx 

        // taken from http://download.intel.com/products/processor/manual/325462.pdf Vol. 2A 3-149
        int cache_type = eax & 0x1F; 

        if (cache_type == 0) // end of valid cache identifiers
            break;

/*
        char * cache_type_string;
        switch (cache_type) {
            case 1: cache_type_string = "Data Cache"; break;
            case 2: cache_type_string = "Instruction Cache"; break;
            case 3: cache_type_string = "Unified Cache"; break;
            default: cache_type_string = "Unknown Type Cache"; break;
        }
*/

        int cache_level = (eax >>= 5) & 0x7;

        int cache_is_self_initializing = (eax >>= 3) & 0x1; // does not need SW initialization
        int cache_is_fully_associative = (eax >>= 1) & 0x1;


        // taken from http://download.intel.com/products/processor/manual/325462.pdf 3-166 Vol. 2A
        // ebx contains 3 integers of 10, 10 and 12 bits respectively
        unsigned int cache_sets = ecx + 1;
        unsigned int cache_coherency_line_size = (ebx & 0xFFF) + 1;
        unsigned int cache_physical_line_partitions = ((ebx >>= 12) & 0x3FF) + 1;
        unsigned int cache_ways_of_associativity = ((ebx >>= 10) & 0x3FF) + 1;

        // Total cache size is the product
        size_t cache_total_size = cache_ways_of_associativity * cache_physical_line_partitions * cache_coherency_line_size * cache_sets;

	// we are only interested in data and unified caches
	if (cache_level == level && (cache_type == 1 || cache_type == 3))
		return cache_total_size;
/*
        printf(
            "Cache ID %d:\n"
            "- Level: %d\n"
            "- Type: %s\n"
            "- Sets: %d\n"
            "- System Coherency Line Size: %d bytes\n"
            "- Physical Line partitions: %d\n"
            "- Ways of associativity: %d\n"
            "- Total Size: %zu bytes (%zu kb)\n"
            "- Is fully associative: %s\n"
            "- Is Self Initializing: %s\n"
            "\n"
            , i
            , cache_level
            , cache_type_string
            , cache_sets
            , cache_coherency_line_size
            , cache_physical_line_partitions
            , cache_ways_of_associativity
            , cache_total_size, cache_total_size >> 10
            , cache_is_fully_associative ? "true" : "false"
            , cache_is_self_initializing ? "true" : "false"
        );
*/
    }
}
