# OpenMP Task Data-Dependency Benchmarks
A set of benchmarks for OpenMP to measure the overhead of task data dependencies under different settings. 
The set of benchmarks includes different task graph patterns that are meant to test different aspects of the implementation, including handling large numbers of dependencies, the type of dependency (input/output/inout), the number of tasks and dependencies per task, and the percentage of matched vs unmatched dependencies. 

The benchmark is based on the [EPCC OpenMP Benchmark Suite](https://www.epcc.ed.ac.uk/research/computing/performance-characterisation-and-benchmarking/epcc-openmp-micro-benchmark-suite) version 3.1. 
In addition to changes to the way task creation and management are timed, we have opted for C++ templates to create tasks with different numbers of dependencies. 
Courtesy to Christoph Niethammer, we have added our own timer class to also measure PAPI counter. 

The current state should be regarded as experimental. While we retained the original benchmark code, we have not verified correctness of the pre-existing set of benchmarks. 
