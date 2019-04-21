//
// Created by israel on 3/28/19.
//

#include "Disk.h"

Disk::Disk() : usingProcess(0) {}

bool Disk::use(int pId) {
    //if disk is not busy use it, else put it to the back of the queue
    if (usingProcess == 0) {
        usingProcess = pId;
        return true;
    } else {
        waiting.push_back(pId);
        return false;
    }
}

void Disk::finishJob() {
    usingProcess = waiting.front();
    waiting.pop_front();
}

int Disk::getProcess() {
    return usingProcess;
}


