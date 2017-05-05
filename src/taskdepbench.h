/******
 *  COPYRIGHT
 *    (C) 2017 HLRS, University of Stuttgart
 *
 * Authors:
 *   Joseph Schuchart
 *   Mathias Nachtmann
 *
 ******/

#ifndef TASKDEPBENCH_H
#define TASKDEPBENCH_H

void refer(void);

template<int NumDeps>
void testTaskDependencyMaster();

template<int NumDeps>
void testTaskDependencyAll();

template<int NumDeps>
void testTaskDependencyFanOutMaster();

template<int NumDeps>
void testTaskDependencyFanIn();

template<int NumDeps>
void testTaskDependencyTree();

template<int NumDeps>
void testTaskDependencyReversedTree();

template<int NumDeps, int stride=1>
void testTaskDependencyNeighbors();

template<int NumDeps>
void testTaskDependencyNeighborsReverse();

template<int NumDeps, int stride=1>
void testTaskDependencyNeighborsMaster();

template<int NumDeps>
void testTaskDependencyNeighborsReverseMaster();

template<int NumDeps>
void testTaskDependencyInChainMaster();

template<int NumDeps>
void testTaskDependencyOutChainMaster();

template<int NumDeps>
void testTaskDependencyInOutChainMaster();

template<int NumDeps>
void testTaskDependencyOutLinkedMaster();

template<int NumDeps>
void testTaskDependencyInLinkedMaster();

template<int NumDeps>
void testTaskDependencyInOutLinkedMaster();

#endif //TASKDEPBENCH_H
