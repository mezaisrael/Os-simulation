//
// Created by israel on 3/28/19.
//

#include "Process.h"

//constructor for the root process it start in the running state since it is the root
Process::Process(int& pId) : _pId(pId), _parentPid(0), _state(running) {
    //increment the availabl pid
    pId++;
}

Process::Process(int& pId, int parentId, ProcState state) : _pId(pId), _parentPid(parentId), _state(state){
//  increment the pid available
    pId++;
}

int Process::getId() {return _pId;}

int Process::getParent() {return _parentPid;}

std::vector<int>& Process::getChildren() {
    return children;
}

void Process::addChild(int childPid) {
    children.push_back(childPid);
}

bool Process::isParent() {
    return !children.empty();
}

void Process::setState(ProcState newState) {
    _state = newState;
}

ProcState Process::getState() const {
    return _state;
}
