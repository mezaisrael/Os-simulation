//
// Created by israel on 3/28/19.
//

#ifndef UNTITLED_PROCESS_H
#define UNTITLED_PROCESS_H

#include <vector>
#include <iostream>
#include "ProcState.h"

class Process {
public:
    //constructor for the root process. It starts int he running state
    //since it is not compting for the cpu.
    Process(int &pId);

    Process(int &pId, int parentId, ProcState state);

    //returns the pid of the process
    int getId();

    int getParent();

    //returns a reference to the Process children
    std::vector<int> & getChildren();

    void addChild(int childPid);

    //returns true if it is a parent of other processes
    bool isParent();

    //set the state of the process
    void setState(ProcState newState);

    //get the state
    ProcState getState() const;
private:

    //process id
    int _pId;

    //state in which the procces is
    ProcState _state;

    //list of pids of its children to it children
    std::vector<int> children;

    //parent id if root it is equal to 0
    int _parentPid;
};


#endif //UNTITLED_PROCESS_H
