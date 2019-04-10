//
// Created by israel on 3/28/19.
//

#ifndef UNTITLED_PROCESS_H
#define UNTITLED_PROCESS_H

#include <vector>
#include <iostream>


class Process {
public:
    //constructors
    Process(int pId);
    Process(int pId, int parentId);

    //returns the pid of the process
    int getId();

    int getParent();

    //forks new process and return the new process created
    //@param childId is the id of the new process created by the fork
    Process fork(int childId);

private:

    //process id
    int _pId;

    //list of pointers to it children
    std::vector<Process*> children;

    //parent id if root it is equal to 0
    int _parentPid;
};


#endif //UNTITLED_PROCESS_H
