//
// Created by israel on 3/28/19.
//

#ifndef UNTITLED_OSSIMULATION_H
#define UNTITLED_OSSIMULATION_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include "Process.h"
#include "Disk.h"
#include "CPU.h"


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

    //list of process control blocks maps Pid to Process
    //maps pid to actual process. this is helpful to have a
    //central place to look for a process regardless of its state
    std::unordered_map<int,Process> pcbs;

    //each disk has process in queue
    std::vector<Disk> disk;

    CPU cpu;

    //processes in waiting
    std::queue<int> waitingQueue;

    //processes in readyQueue queue
    std::queue<int> readyQueue;

    //id of terminated process
    std::vector<int> terminated;

    //new process begins
    void startNewProcess();

    //displays snapshot of readyQueue queue
    void snapShotReadyQueue() const;

    //current running process use time is up, run the next
    //process in the ready queue
    void rotateProcess();

    //forks the current process and returns it the child
    Process forkRunning();
};


#endif //UNTITLED_OSSIMULATION_H
