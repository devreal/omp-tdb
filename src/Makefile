include Makefile.defs

# If CFLAGS_CRAY is empty set it to CFLAGS
ifeq ($(CFLAGS_CRAY),)
CFLAGS_CRAY = ${CFLAGS}
endif

.c.o:
	${CC} ${CFLAGS} $(OMPFLAG) -c $*.c 
	
.cc.o:
	${CXX} ${CFLAGS} $(OMPFLAG) -c $*.cc 

SYNCOBJS =  syncbench.o common.o 
SCHEDOBJS = schedbench.o common_sched.o 
ARRAYOBJS = arraybench_$(IDA).o common.o 
TASKOBJS =  taskbench.o common.o 
TASDEPKOBJS =  taskdepbench.o common.o 
SCHEDFLAGS = -DSCHEDBENCH

all:	syncbench schedbench taskbench taskdepbench
	$(MAKE) IDA=1 prog
	$(MAKE) IDA=3 prog
	$(MAKE) IDA=9 prog
	$(MAKE) IDA=27 prog
	$(MAKE) IDA=81 prog
	$(MAKE) IDA=243 prog
	$(MAKE) IDA=729 prog
	$(MAKE) IDA=2187 prog
	$(MAKE) IDA=6561 prog
	$(MAKE) IDA=19683 prog
	$(MAKE) IDA=59049 prog

prog: arraybench_$(IDA) 

syncbench: $(SYNCOBJS)
	$(CXX) -o syncbench $(SYNCOBJS) $(CLOCKOBJS) $(LIBS) -lm $(LDFLAGS)

# Rule to ensure the lower optimisation level is picked up for common.c 
# with the Cray compiler
common.o:	
	${CXX} ${CFLAGS_CRAY} $(OMPFLAG) -c $*.cc 

# Separate rule to build common_sched.o as we need to ensure the correct 
# DEFAULT_DELAY_TIME is used. 
common_sched.o:	
	${CXX} ${CFLAGS_CRAY} $(SCHEDFLAGS) $(OMPFLAG) -o common_sched.o -c common.cc

schedbench: $(SCHEDOBJS)
	$(CXX) -o schedbench  $(SCHEDOBJS) $(CLOCKOBJS) $(LIBS) -lm $(LDFLAGS)

# Multiple header files due to multiple array sizes, makes header file arraybench_*.h
arraybench_$(IDA).h: arraybench.h
	$(CPP) -DIDA=$(IDA) $(OMPFLAG) -P arraybench.h -o $@

# Multiple object files due to multiple array sizes, makes object file arraybench_*.o
arraybench_$(IDA).o: arraybench_$(IDA).h arraybench.c
	$(CC) $(CFLAGS) -DIDA=$(IDA) $(OMPFLAG) arraybench.c -c -o $@

# Multiple executables due to multiple array sizes, makes exe file arraybench_*
arraybench_$(IDA): $(ARRAYOBJS) $(CLOCKOBJS) arraybench.c
	$(CXX) $(ARRAYOBJS) $(CLOCKOBJS) $(LIBS) -lm -o $@ $(LDFLAGS)

taskbench: $(TASKOBJS)
	$(CXX) -o taskbench $(OMPFLAG) $(TASKOBJS) $(CLOCKOBJS) $(LIBS) -lm $(LDFLAGS) 

taskdepbench: $(TASKOBJS)
	$(CXX) -o taskdepbench $(OMPFLAG) $(TASKOBJS) $(CLOCKOBJS) $(LIBS) -lm $(LDFLAGS) 

clean: 
	-rm *.o syncbench schedbench arraybench_* taskbench taskdepbench

clean-all: clean 	
	-rm OpenMPBench.* *.all


