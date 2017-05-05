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

#ifndef COMMON_H
#define COMMON_H

#define DEFAULT_DELAY_LENGTH -1 // -1 means the delay length should be auto generated
#define DEFAULT_OUTER_REPS 20   // Outer repetitions
#define DEFAULT_TEST_TARGET_TIME 1000.0 // Test Target time in microseconds.
#ifdef SCHEDBENCH
#define DEFAULT_DELAY_TIME 15.0  // Default delaytime in microseconds for schedbench
#else
#define DEFAULT_DELAY_TIME 0.10  // Default delaytime in microseconds
#endif

#ifdef __cplusplus
extern "C"{
#endif

extern int nthreads;              // Number of OpenMP threads
extern int delaylength;           // The number of iterations to delay for
extern int outerreps;             // Outer repetitions
extern unsigned long innerreps;   // Inner repetitions
extern double delaytime;          // Delay time in microseconds
extern double targettesttime;     // The length of time in microseconds the test
                                  // should run for
extern int numberoftasks;         // The number of tasks to create for each inner rep


void init(int argc, char **argv);

void initreference(const char *name);

void finalisereference(const char *name);

void inittest(const char *name);

void finalisetest(const char *name);

double getclock();

void delay(int delaylength);

void array_delay(int delaylength, double a[1]);

void delay_l2clear(int delaylength);

int getdelaylengthfromtime(double delaytime);

int returnfalse(void);

void finalise(void);

void benchmark(const char *name, void (*test)(void));

void reference(const char *name, void (*refer)(void));


#ifdef __cplusplus
}
#endif

#endif //COMMON_H
