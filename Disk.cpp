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
    int next;
    if (waiting.empty()) {
        next = 0;
    } else {
        next = waiting.front();
        waiting.pop_front();
    }
    usingProcess = next;
}

int Disk::getProcess() {
    return usingProcess;
}

std::deque<int> &Disk::getQueue() {
    return waiting;
}

bool Disk::isIdle() {
    return usingProcess == 0;
}

void Disk::remove(int pid) {
    for (auto it = waiting.begin(); it < waiting.end(); it++) {
        if (*it == pid) {
            //remove from queue
            waiting.erase(it);
        }
    }
}


