#!/bin/bash

#PBS -lnodes=1,walltime=24:00:00
#PBS -n 
#PBS -N omp_taskbench_jobsize
#PBS -m ae
#PBS -j oe

if [ -n "$PBS_O_WORKDIR" ] ; then
cd $PBS_O_WORKDIR
fi

pwd

export KMP_AFFINITY=verbose

CLANG_PATH="/zhome/academic/HLRS/hlrs/hpcjschu/opt/clang-3.9.1/"

function modlistgrep() {
	module list 2>&1 | tr " (" '\n' | grep $1
}

#COMPILER="gnu/5.3.0 gnu/6.3.0 intel/16.0.3.210 intel/17.0.1.132"
#NUM_TASKS="1000 3000 10000 100000"
#NUM_TASKS="1000 5000 10000 50000 100000 500000 1000000"
NUM_TASKS="1000 5000"
NUM_TASKS="1000"
NUM_THREADS="1 2"
#NUM_THREADS="2 12"
#NUM_THREADS="12"
#NUM_THREADS="1"

MIN_TASKSIZE=.1
MAX_TASKSIZE=15

LAUNCHER=""
RESULTDIR="taskbench_jobsize_$(date +%Y%m%d-%H-%M)_${PBS_JOBID}"
RESULTFILE="UNKNOWN_COMPILER"

if [[ $(hostname) == cl3fr* ]]
then
	HOSTNAME="laki"
	COMPILER="gnu/5.3.0 gnu/6.3.0 intel/16.0.3 intel/17.0.1"
	module load performance/papi/2015-09-09-git85a9312
else
	HOSTNAME="hazelhen"
	COMPILER="gnu clang intel intel/17.0.1.132 mcc.ompss mcc.openmp"
	COMPILER="mcc.ompss mcc.openmp"
#	COMPILER="${COMPILER} clang cray mcc.ompss mcc.openmp"
	LAUNCHER="aprun -n 1 -cc numa_node"
	module load papi
fi

if [ -n "$user_compiler" ] ; then
	COMPILER="$user_compiler"
fi

echo "Chosen compilers to test: $COMPILER"

echo "Creating result dir: ${RESULTDIR}"
mkdir ${RESULTDIR}

function switch-compiler {
	local compiler="${1%\/*}"
	local version="${1#*\/}"
	if [ "$version" == "$compiler" ] ; then
		version=""
	fi
#	cp Makefile.defs.${HOSTNAME}.${compiler} Makefile.defs || echo "Unknown compiler" && exit 1
	cp Makefile.defs.${HOSTNAME}.${compiler} Makefile.defs 
	RESULTFILE="taskbench_${1/\//-}"
	if [ $HOSTNAME == "hazelhen" ]
	then
		echo "Switching to compiler $compiler"
		if [ $compiler == "clang" ] ; then
			echo "Using Clang"
			# Clang compiler only available locally
			export PATH=${CLANG_PATH}/bin:$PATH LD_LIBRARY_PATH=${CLANG_PATH}/lib:$LD_LIBRARY_PATH
			module load gcc/5.3.0
		elif [[ $compiler == mcc* ]] ; then
			echo "Using OmpSs"
			export PATH=/zhome/academic/HLRS/hlrs/hpcmnach/hornet/opt/ompss-16.06.3/bin:$PATH
			module del gcc
			module load gcc/5.3.0
		else 
			echo "Switching from $(modlistgrep PrgEnv) to PrgEnv-${1} (${compiler} version ${version})"
			module swap $(modlistgrep PrgEnv) PrgEnv-${compiler}
			# load the separate compiler module 
			# in case a different version was requested
			if [ -n "${version}" ] ; then 
				module del ${compiler}
				module load ${1}
			fi
		fi 
	else 
		# on laki we just load a different module (Clang not available)
		echo "Switching from $(modlistgrep compiler) to ${1} (${compiler} version ${version})"
		module del $(modlistgrep compiler)
		module load compiler/${1}
	fi
}

for compiler in $COMPILER 
do
	# build using the specific compiler
	switch-compiler $compiler
	make clean && time make -j4 taskbench || exit
	
	for numtasks in $NUM_TASKS ; do
		for numthreads in $NUM_THREADS ; do
			delay="$MIN_TASKSIZE"
			while (( $(echo "$delay<$MAX_TASKSIZE" | bc) )) ; do
				echo "Executing benchmark with ${compiler} compiler with ${numtasks} tasks on ${numthreads} threads and delay $delay"
				export OMP_NUM_THREADS=${numthreads}
				echo "$LAUNCHER ./taskbench --single --num-tasks ${numtasks} --delay-time 0${delay} &> ${RESULTDIR}/${RESULTFILE}_t${numthreads}_n${numtasks}_d0${delay}.log"
				time $LAUNCHER ./taskbench --single --num-tasks ${numtasks} --delay-time 0${delay} &> "${RESULTDIR}/${RESULTFILE}_t${numthreads}_n${numtasks}_d0${delay}.log"
				echo "Executing bc"
				delay=$( echo "${delay}*2" | bc )
			done
		done 
	done
done


# make the results group readable
chmod -R g+rX ${RESULTDIR}
