//
// Created by israel on 3/28/19.
//

#include "Process.h"

//constructor for the root process
Process::Process(int pId) : _pId(pId), _parentPid(0) {}

Process::Process(int pId, int parentId) : _pId(pId), _parentPid(parentId) {}

int Process::getId() {return _pId;}

int Process::getParent() {return _parentPid;}

std::vector<Process*>& Process::getChildren() {
    return children;
}

void Process::addChild(Process *child) {
    children.push_back(child);
}