//
// Created by israel on 3/28/19.
//

#ifndef UNTITLED_PROCESS_H
#define UNTITLED_PROCESS_H

#include <iostream>
#include <unordered_set>
#include <deque>
#include <bits/unordered_set.h>
#include "ProcState.h"
#include "Disk.h"

class Process {
public:
    //constructor for the root process. It starts int he running state
    //since it is not competing for the cpu.
    Process(int &pId);

    Process(int &pId, int parentId, ProcState state);

    //returns the pid of the process
    int getId();

    int getParent();

    //returns a reference to the Process children
    std::unordered_set<int> & getChildren();

    void addChild(int childPid);

    //returns true if it is a parent of other processes
    bool isParent();

    //set the state of the process
    void setState(ProcState newState);

    //get the state
    ProcState getState() const;

    void removeChild(int childId);

    void requestDisk(Disk &disk, std::string & fileName);

    std::string getFile();
private:

    //process id
    int _pId;

    //state in which the process is
    ProcState _state;

    //list of pids of its children to it children
    std::unordered_set<int> children;

    //parent id if root it is equal to 0
    int _parentPid;

    //the number of the disk it is in. If it is not
    //in disk queue set it to -1
    int currentDisk;

    //the name of the file it is using. Empty string if
    //it is not using a file
    std::string _fileName;
};


#endif //UNTITLED_PROCESS_H
