//
// Created by israel on 3/28/19.
//

#ifndef UNTITLED_OSSIMULATION_H
#define UNTITLED_OSSIMULATION_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <deque>
#include "Process.h"
#include "Disk.h"
#include "CPU.h"
#include "HelperFunctions.h"
#include "MemoryManager.h"



class OsSimulation {
public:
    OsSimulation();

    //function that prompts the user for a command
    void promptForCommand();
private:
    //pid of the root process
    const int rootPid = 1;
    //current _pId available
    int pIdAvailable;

    //list of process control blocks. maps Pid to Process it
    //is helpful to have a central place to look for a process regardless of its state
    std::unordered_map<int,Process> processControl;

    //each disk has process in queue
    std::vector<Disk> disks;

    CPU cpu;

    //memory
    MemoryManager _memoryManager;

    //processes in readyQueue queue
    std::deque<int> readyQueue;

    //id of terminated process
    std::vector<int> terminatedProc;

    //--- private functions ------------------------

    //return reference to the running process
    Process &runningProcess();


    //new process begins
    void startNewProcess();

    //displays snapshot of readyQueue queue
    void snapShotReadyQueue() const;

    //current running process use time is up, run the next
    //process in the ready queue
    void rotateProcess();

    //forks parentProcess
    void fork(Process &parentProcess);

    void waitForChildren();

    void printProcessInfo();

    //exist the current running process.
    //if it has children exit them also
    //if its parent is in the wait state move it to the ready queue
    void exitRunning();

    //helper method for exitRunning()
    void terminate(Process &processPtr);

    /*
    recursive helper method called by exit running infectedProcess
     is the process that is about to turn into zombie
    */
    void turnToZombie(Process &infectedProcess);

    //dIndex is the index of the disk filename wants to use
    void requestFile(int dIndex, std::string &fileName);

    void diskFinish(int num);

    void snapShotIO();

    void runNextInQueue();

    void requestMemory(int address);

    void removeFromReady(int id);
};


#endif //UNTITLED_OSSIMULATION_H
