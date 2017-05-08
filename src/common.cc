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
*---------------------------------------------------------------------------*
*                        With modifications by                              *
*                                                                           *
*                Joseph Schuchart, Mathias Nachtmann                        *
*                                                                           *
*                                at                                         *
*                                                                           *
*                    HLRS, University of Stuttgart                          *
*                                                                           *
*         email: schuchart@hlrs.de or nachtmann@hlrs.de                     *
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "common.h"
#include "Timer.h"
#include "compiler.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

#define CONF95 1.96

int nthreads = -1;           // Number of OpenMP threads
int delaylength = -1;        // The number of iterations to delay for
int outerreps = -1;          // Outer repetitions
double delaytime = -1.0;     // Length of time to delay for in microseconds
double targettesttime = 0.0; // The length of time in microseconds that the test
                             // should run for.
int numberoftasks = 1000; 
unsigned long innerreps; // Inner repetitions
std::vector<double> times;           // Array of doubles storing the benchmark times in microseconds
double referencetime;    // The average reference time in microseconds to perform

#if WITH_PAPI
#endif // WITH_PAPI

			 // outerreps runs
double referencesd;      // The standard deviation in the reference time in
			 // microseconds for outerreps runs.
double testtime;         // The average test time in microseconds for
			 // outerreps runs
double testsd;		 // The standard deviation in the test time in
			 // microseconds for outerreps runs.

double local_papi_cyc = 0.0, local_papi_tcm = 0.0, local_papi_ins = 0.0;

Timer timer;

uint64_t delaycnt;

std::vector<Timer> local_timer;

int single = 0;

#ifdef WITH_PAPI
#if !defined(__PGI) && !defined(__INTEL_COMPILER) 
std::vector<const char *> papi_ctr_names{"PAPI_TOT_CYC", "PAPI_TOT_INS", "PAPI_L2_TCM"};
#else
// PGI does not support initialization using braces :(
std::vector<const char *> papi_ctr_names;
#endif
std::vector<double> papi_ctr[3];
double papi_reference_val[3];
double papi_reference_sd[3];
#endif

static void print_stat_header();

void usage(char *argv[]) {
    printf("Usage: %s \n"
	   "\t--outer-repetitions <outer-repetitions> (default %d)\n"
	   "\t--test-time <target-test-time> (default %0.2f microseconds)\n"
	   "\t--delay-time <delay-time> (default %0.4f microseconds)\n"
	   "\t--delay-length <delay-length>"
	   "(default auto-generated based on processor speed)\n"
       "\t--num-tasks <number of tasks per inner rep> (default 1000)\n"
       "\t--single (force all but the root thread to wait, default no)\n",
	   argv[0],
	   DEFAULT_OUTER_REPS, DEFAULT_TEST_TARGET_TIME, DEFAULT_DELAY_TIME);
}


void parse_args(int argc, char *argv[]) {
    // Parse the parameters
    int arg;
    for (arg = 1; arg < argc; arg++) {
	if (strcmp(argv[arg], "--delay-time") == 0) {
	    delaytime = atof(argv[++arg]);
	    if (delaytime == 0.0) {
		printf("Invalid float:--delay-time: %s\n", argv[arg]);
		usage(argv);
		exit(EXIT_FAILURE);
	    }

	} else if (strcmp(argv[arg], "--outer-repetitions") == 0) {
	    outerreps = atoi(argv[++arg]);
	    if (outerreps == 0) {
		printf("Invalid integer:--outer-repetitions: %s\n", argv[arg]);
		usage(argv);
		exit(EXIT_FAILURE);
	    }

	} else if (strcmp(argv[arg], "--test-time") == 0) {
	    targettesttime = atof(argv[++arg]);
	    if (targettesttime == 0) {
		printf("Invalid integer:--test-time: %s\n", argv[arg]);
		usage(argv);
		exit(EXIT_FAILURE);
	    }

	} else if (strcmp(argv[arg], "--num-tasks") == 0) {
	    numberoftasks = atoi(argv[++arg]);
	    if (numberoftasks == 0) {
		printf("Invalid integer:--num-tasks: %s\n", argv[arg]);
		usage(argv);
		exit(EXIT_FAILURE);
	    }

	} else if (strcmp(argv[arg], "--single") == 0) {
	    single = 1;
	} else if (strcmp(argv[arg], "-h") == 0) {
	    usage(argv);
	    exit(EXIT_SUCCESS);

	} else {
	    printf("Invalid parameters: %s\n", argv[arg]);
	    usage(argv);
	    exit(EXIT_FAILURE);
	}
    }
}

int getdelaylengthfromtime(double delaytime) {
    int i, reps;
    double lapsedtime, starttime; // seconds

    reps = 1000;
    lapsedtime = 0.0;

    delaytime = delaytime/1.0E6; // convert from microseconds to seconds

    // Note: delaytime is local to this function and thus the conversion
    // does not propagate to the main code.

    // Here we want to use the delaytime in microseconds to find the
    // delaylength in iterations. We start with delaylength=0 and
    // increase until we get a large enough delaytime, return delaylength
    // in iterations.

    delaylength = 0;
    delay(delaylength);

    while (lapsedtime < delaytime) {
	delaylength = delaylength * 1.1 + 1;
	timer.reset();
	timer.start();
	for (i = 0; i < reps; i++) {
	    delay(delaylength);
	}
	timer.stop();
	lapsedtime = (timer.get_etime()) / (double) reps;
    }
    return delaylength;

}

unsigned long getinnerreps(void (*test)(void)) {
    innerreps = 1L;  // some initial value
    double time = 0.0;

    while (time < targettesttime) {
	innerreps *=2;
	timer.reset();

#pragma omp parallel
	{
#pragma omp barrier
#ifndef OMPSS
#pragma omp master
#endif
        { timer.start(); }
#pragma omp barrier
	test();
#pragma omp taskwait
#pragma omp barrier
#ifndef OMPSS
#pragma omp master
#endif
        { timer.stop(); }
	}
	time = (timer.get_etime()) * 1.0e6;

	// Test to stop code if compiler is optimising reference time expressions away
	if (innerreps > (targettesttime*1.0e15)) {
	    printf("Compiler has optimised reference loop away, STOP! \n");
	    printf("Try recompiling with lower optimisation level \n");
	    exit(1);
	}
    }
    return innerreps;
}

void printheader(const char *name) {
    printf("\n");
    printf("--------------------------------------------------------\n");
    printf("Computing %s time using %lu innerreps and %i tasks\n", name, innerreps, numberoftasks);
}

template<typename T>
void stats(const std::vector<T>& values, T& mean, T& min, T& max, T& sd, int& nr) {

    T total;

    // we skip the first entry
    min = *std::min_element(++(values.begin()), values.end());
    max = *std::max_element(++(values.begin()), values.end());
    total = std::accumulate(++(values.begin()), values.end(), static_cast<T>(0));
    mean = total / static_cast<T>(outerreps);

/*
    for (int i = 1; i <= outerreps; i++) {
	mintime = (mintime < times[i]) ? mintime : times[i];
	maxtime = (maxtime > times[i]) ? maxtime : times[i];
	totaltime += times[i];
    }

    meantime = totaltime / outerreps;
*/

    T sumsq = 0;

    for (int i = 1; i <= outerreps; i++) {
	sumsq += (values[i] - mean) * (values[i] - mean);
    }
    sd = std::sqrt((double)sumsq / (outerreps - 1));

    T cutoff = 3 * sd;

    nr = std::count_if(++(values.begin()), values.end(),
		[&](T val){ return std::abs(val - mean) > cutoff; }
	 );
/*
    for (i = 1; i <= outerreps; i++) {
	if (fabs(times[i] - meantime) > cutoff)
	    nr++;
    }
*/

/*
    printf("\n");
    printf("Sample_size       Average     Min         Max          S.D.          Outliers\n");
    printf(" %d                %f   %f   %f    %f      %d\n",
	   outerreps, meantime, mintime, maxtime, sd, nr);
    printf("\n");

    *mtp = meantime;
    *sdp = sd;
*/
}

#ifdef WITH_PAPI
void papi_stats(int ctr_id) {

    double meantime, totaltime, sumsq, mintime, maxtime, sd=0.0, cutoff;

    int i, nr = 0;

    mintime = 1.0e10;
    maxtime = 0.;
    totaltime = 0.;
/*
    for (i = 1; i <= outerreps; i++) {
	mintime = (mintime < times[i]) ? mintime : times[i];
	maxtime = (maxtime > times[i]) ? maxtime : times[i];
	totaltime += times[i];
    }
*/
    long long min_val = *std::min_element(++(papi_ctr[ctr_id].begin()), papi_ctr[ctr_id].end());
    long long max_val = *std::max_element(++(papi_ctr[ctr_id].begin()), papi_ctr[ctr_id].end());
//    long long unsigned tot_val = std::accumulate(++(papi_ctr[ctr_id].begin()), papi_ctr[ctr_id].end(), 0);
    double mean_val = 0.0;
//    long long unsigned mean_val= tot_val / outerreps;
    for (size_t i = 1; i <= outerreps; ++i) {
	mean_val += (double)papi_ctr[ctr_id][i] / (double)outerreps;
    }

/*
    meantime = totaltime / outerreps;
    sumsq = 0;

    for (i = 1; i <= outerreps; i++) {
	sumsq += (times[i] - meantime) * (times[i] - meantime);
    }
    sd = sqrt(sumsq / (outerreps - 1));

    cutoff = 3.0 * sd;

    nr = 0;

    for (i = 1; i <= outerreps; i++) {
	if (fabs(times[i] - meantime) > cutoff)
	    nr++;
    }
*/
    printf("\n");
    printf("Counter: %s\n", papi_ctr_names[ctr_id]);
    printf("Sample_size       Average     Min         Max          S.D.          Outliers\n");
    printf(" %d                %f   %llu   %llu    %f      %d\n",
	   outerreps, mean_val, min_val, max_val, sd, nr);
    printf("\n");

}
#endif



void printfooter(const char *name, double testtime, double testsd,
		 double referencetime, double refsd) {
    printf("\n");
    printf("%s time     = %f microseconds +/- %f\n",
	   name, testtime, CONF95*testsd);
    printf("%s overhead = %f microseconds +/- %f\n",
	   name, testtime-referencetime, CONF95*(testsd+referencesd));

}

void printreferencefooter(const char *name, double referencetime, double referencesd) {
    printf("%s time     = %f microseconds +/- %f\n",
	   name, referencetime, CONF95 * referencesd);
}

void init(int argc, char **argv)
{
#pragma omp parallel
    {
#ifndef OMPSS
#pragma omp master
#endif
	{
	    nthreads = omp_get_num_threads();
	}

    }

    parse_args(argc, argv);

    if (outerreps == -1) {
	outerreps = DEFAULT_OUTER_REPS;
    }
    if (targettesttime == 0.0) {
	targettesttime = DEFAULT_TEST_TARGET_TIME;
    }
    if (delaytime < 0.0) {
	delaytime = DEFAULT_DELAY_TIME;
    }
    delaylength = getdelaylengthfromtime(delaytime); // Always need to compute delaylength in iterations

    //times = (double*)malloc((outerreps+1) * sizeof(double));
    times.reserve(outerreps + 1);
#ifdef WITH_PAPI
#if defined(__PGI) || defined(__INTEL_COMPILER)
    papi_ctr_names.push_back("PAPI_TOT_CYC");
    papi_ctr_names.push_back("PAPI_TOT_INS");
    papi_ctr_names.push_back("PAPI_L2_TCM");
#endif
    for (int i = 0; i < papi_ctr_names.size(); i++) {
        papi_ctr[i].reserve(outerreps + 1);
    }
    printf("Adding %i counter to PAPI set\n", papi_ctr_names.size());
//    timer.add_papi_counters(papi_ctr_names.size(), papi_ctr_names.data());
#endif

    local_timer.resize(nthreads);
#pragma omp parallel
    {
	int threadnum = omp_get_thread_num();
#ifdef WITH_PAPI
	local_timer[threadnum].add_papi_counters(papi_ctr_names.size(), papi_ctr_names.data());
#endif
	local_timer[threadnum].reset();
    }


    printf("Running OpenMP benchmark version 3.0\n"
           "Compiled with %s\n"
	   "\t%d thread(s) %s\n"
	   "\t%d outer repetitions\n"
	   "\t%0.2f test time (microseconds)\n"
	   "\t%d delay length (iterations) \n"
	   "\t%f delay time (microseconds)\n",
	   COMPILER_STR, nthreads, (single) ? "(single mode)" : "",
	   outerreps, targettesttime,
	   delaylength, delaytime);
}

void finalise(void) {
}

void initreference(const char *name) {
    printheader(name);
    times.clear();
#ifdef WITH_PAPI
    for (int i = 0; i < papi_ctr_names.size(); i++) {
        papi_ctr[i].clear();
    }
#endif
}

/* Calculate the reference time. */
void reference(const char *name, void (*refer)(void)) {
    int k;
    double start;

    // Calculate the required number of innerreps
    innerreps = getinnerreps(refer);

    initreference(name);

    for (k = 0; k <= outerreps; k++) {
	timer.reset();
	local_papi_cyc = 0, local_papi_tcm = 0, local_papi_ins = 0;
	int single_flag = 0;
#pragma omp parallel
	{
	int threadnum = omp_get_thread_num();
	local_timer[threadnum].reset();
#pragma omp master 
	{
	timer.start();
	}
#pragma omp barrier
	local_timer[threadnum].start();
	if (!single || threadnum == 0) {
	    refer();
	    single_flag = 1;
#pragma omp flush(single_flag)
	} else {
	    // wait for the single_flag to signal
	    while(!single_flag) {
		#pragma omp flush(single_flag)
		;
	    }
	}
	local_timer[threadnum].stop();
#ifndef OMPSS
#pragma omp master
#endif
	{
    	timer.stop();
	}
#ifdef WITH_PAPI
#pragma omp atomic
	local_papi_cyc += local_timer[threadnum].get_papi_counter(0);
#pragma omp atomic
	local_papi_ins += local_timer[threadnum].get_papi_counter(1);
#pragma omp atomic
	local_papi_tcm += local_timer[threadnum].get_papi_counter(2);
#endif
	}

	times.push_back((timer.get_etime()) * 1.0e6 / (double) innerreps / numberoftasks);
#ifdef WITH_PAPI
	papi_ctr[0].push_back(local_papi_cyc / innerreps / numberoftasks);
	papi_ctr[1].push_back(local_papi_ins / innerreps / numberoftasks);
	papi_ctr[2].push_back(local_papi_tcm / innerreps / numberoftasks);
#endif
    }

    finalisereference(name);

}

static void
print_stat_header()
{
//    printf("Type   Sample_size       Average     Min         Max          S.D.    CONF95   OVERHEAD OVERHEAD_CONF95   Outliers\n");
    std::cout << "\n"
              << std::setw(10) << "Type" << std::setw(15)
              << std::setw(15) << "Sample_size"
              << std::setw(15) << "Average"
              << std::setw(15) << "Min"
              << std::setw(15) << "Max"
              << std::setw(15) << "S.D."
              << std::setw(15) << "CONF95"
              << std::setw(15) << "OVERHEAD"
              << std::setw(20) << "OVERHEAD_CONF95"
              << std::setw(15) << "Outliers" << std::endl;
}

template<typename T>
static void
print_stats(const char* name, T mean, T min, T max, T sd, T reference, T referencesd, int nr)
{
/*    printf("%s  %d                %f   %f   %f    %f      %d\n",
	   name, outerreps, mean, min, max, sd, nr);
*/
    std::cout << std::left;
    std::cout.width(15);
    std::cout << name
	<< std::right << std::setprecision(5)
	<< std::setw(10) << outerreps
	<< std::setw(15) << mean
	<< std::setw(15) << min
	<< std::setw(15) << max
	<< std::setw(15) << sd
	<< std::setw(15) << CONF95*sd
	<< std::setw(15) << mean-reference
	<< std::setw(20) << CONF95*(sd+referencesd)
	<< std::setw(15) << nr << std::endl;
}

void finalisereference(const char *name) {
    //stats(&referencetime, &referencesd);
    double min, max;
    int nr;
    stats(times, referencetime, min, max, referencesd, nr);
    print_stat_header();
    print_stats("TIME", referencetime, min, max, referencesd, 0.0, 0.0, nr);
#ifdef WITH_PAPI
    for (int i = 0; i < papi_ctr_names.size(); i++) {
        stats(papi_ctr[i], papi_reference_val[i], min, max, papi_reference_sd[i], nr);
        print_stats(papi_ctr_names[i], papi_reference_val[i], min, max, papi_reference_sd[i], 0.0, 0.0, nr);
        //papi_stats(i);
    }
#endif

    printreferencefooter(name, referencetime, referencesd);
}

void inittest(const char *name) {
    printheader(name);
    times.clear();
#ifdef WITH_PAPI
    for (int i = 0; i < papi_ctr_names.size(); i++) {
        papi_ctr[i].clear();
    }
#endif
}

void finalisetest(const char *name) {

    print_stat_header();
    double min, max;
    int nr;
    stats(times, testtime, min, max, testsd, nr);
    print_stats("TIME", testtime, min, max, testsd, referencetime, referencesd, nr);
#ifdef WITH_PAPI
    for (int i = 0; i < papi_ctr_names.size(); i++) {
	double mean, min, max, sd;
        int nr;
        stats(papi_ctr[i], mean, min, max, sd, nr);
        print_stats(papi_ctr_names[i], mean, min, max, sd, papi_reference_val[i], papi_reference_sd[i], nr);
        //papi_stats(i);
    }
#endif
    printf("DELAYCNT: %lu\n", delaycnt);
    printfooter(name, testtime, testsd, referencetime, referencesd);

}

/* Function to run a microbenchmark test*/
void benchmark(const char *name, void (*test)(void))
{
    int k;
    double start;

    // Calculate the required number of innerreps
    //std::cout << "getinnerreps"<< innerreps;
    //innerreps = getinnerreps(test);
    innerreps = 1;
    std::cout << "TEST";
    inittest(name);

    delaycnt = 0;


    for (k=0; k<=outerreps; k++) {
	timer.reset();
	local_papi_cyc = 0, local_papi_tcm = 0, local_papi_ins = 0;
	int single_flag = 0;
#pragma omp parallel
{
	int threadnum = omp_get_thread_num();
	local_timer[threadnum].reset();
#pragma omp barrier
#ifndef OMPSS
#pragma omp master
#endif
	{
	timer.start();
	}
#pragma omp barrier
	local_timer[threadnum].start();
	if (!single || threadnum == 0) {
	    test();
	    single_flag = 1;
	    #pragma omp flush(single_flag)
	} else {
	    // busy wait for the single flag to signal
	    while (!single_flag) {
		#pragma omp flush(single_flag)
	    }
	}
	local_timer[threadnum].stop();
#pragma omp barrier
#ifndef OMPSS
#pragma omp master
#endif
	{
	timer.stop();
	}
#ifdef WITH_PAPI
#pragma omp atomic
	local_papi_cyc += local_timer[threadnum].get_papi_counter(0);
#pragma omp atomic
	local_papi_ins += local_timer[threadnum].get_papi_counter(1);
#pragma omp atomic
	local_papi_tcm += local_timer[threadnum].get_papi_counter(2);
#endif //WITH_PAPI
}

	times.push_back((timer.get_etime()) * 1.0e6 / (double) innerreps / numberoftasks);


#ifdef WITH_PAPI
	papi_ctr[0].push_back(local_papi_cyc / innerreps / numberoftasks);
	papi_ctr[1].push_back(local_papi_ins / innerreps / numberoftasks);
	papi_ctr[2].push_back(local_papi_tcm / innerreps / numberoftasks);
#endif
    }

    finalisetest(name);
}

// For the Cray compiler on HECToR we need to turn off optimisation
// for the delay and array_delay functions. Other compilers should
// not be afffected.
#if defined(_CRAYC)
#pragma _CRI noopt
#elif defined(__INTEL_COMPILER)
#pragma optimize("", off) 
#else 
#pragma GCC optimize 0
#endif
void delay(int delaylength) {

    int i;
    float a = 0.;

    for (i = 0; i < delaylength; i++)
	a += i;
    if (a < 0)
	printf("%f \n", a);

    __sync_add_and_fetch(&delaycnt, 1UL);

}

void array_delay(int delaylength, double a[1]) {

    int i;
    a[0] = 1.0;
    for (i = 0; i < delaylength; i++)
	a[0] += i;
    if (a[0] < 0)
	printf("%f \n", a[0]);

}

// Re-enable optimisation for remainder of source.
#if defined(_CRAYC)
#pragma _CRI opt
#elif defined(__INTEL_COMPILER)
#pragma optimize("", on) 
#else 
#pragma GCC optimize 1
#endif

double getclock() {
    double time;
    // Returns a value in seconds of the time elapsed from some arbitrary,
    // but consistent point.
    double omp_get_wtime(void);
    time = omp_get_wtime();
    return time;
}

int returnfalse() {
    return 0;

}
