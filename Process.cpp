//
// Created by israel on 3/28/19.
//

#include "Process.h"

//constructor for the root process it start in the running state since it is the root
Process::Process(int& pId) : _pId(pId), _parentPid(0), _state(running), currentDisk(-1) {
    //increment the available pid
    pId++;
}

Process::Process(int& pId, int parentId, ProcState state) : _pId(pId), _parentPid(parentId), _state(state), currentDisk(
        -1){
//  increment the pid available
    pId++;
}

int Process::getId() {return _pId;}

int Process::getParent() {return _parentPid;}

std::unordered_set<int>& Process::getChildren() {
    return children;
}

void Process::addChild(int childPid) {
    children.emplace(childPid);
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

void Process::removeChild(int childId) {
    children.erase(childId);
}

void Process::requestDisk(Disk &disk, std::string & fileName) {
    _fileName = fileName;

    bool inQueue = disk.use(_pId);
    if (inQueue) {
        setState(readyIO);
    } else {
        setState(usingDisk);
    }
}

std::string Process::getFile() {
    return _fileName;
}

void Process::finishUsingDisk() {
    currentDisk = -1;
    _fileName = "";
    setState(ready);
}


