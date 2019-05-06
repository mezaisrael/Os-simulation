//
// Created by israel on 5/2/19.
//

#include <iostream>
#include "MemoryManager.h"

MemoryManager::MemoryManager() : _time(1), _lruPosition(0) {

}

void MemoryManager::initiate(int memory, int pgSize) {
    _memory = memory;
    _pageSize = pgSize;

    _frames.resize(_memory/_pageSize);
}

void MemoryManager::request(int address, int pId) {
    _time++;
    int pageNum = address/_pageSize;

    //check if the process with page # is not already in use
    for(int i = 0 ; i < _frames.size(); i++) {
        Frame &currFrame = _frames.at(i);
        if (currFrame.getPid() == pId && currFrame.getPageNumber() == pageNum) {
            //update the time stamp
            currFrame.setTime(_time);
            return;
        }

        //also check for the oldest frame used
        int lruTime = getLru().lastUsed();
        if (currFrame.lastUsed() < lruTime) {
            _lruPosition = i;
        }
    }

    //if it pid and pg# is not in a frame we should replace with lru
    Frame & lru = _frames.at(_lruPosition);
    lru.setFrame(pId, pageNum, _time);
}

void MemoryManager::snapshot() {
    std::cout << "Frame|pg#|pid|timeStamp"<< std::endl;
    for(int i = 0; i < _frames.size(); i ++) {
        Frame& currFrame = _frames.at(i);

        if(currFrame.isEmpty())  {
            continue;
        }

        int pgNum = currFrame.getPageNumber();
        int pid = currFrame.getPid();
        int time = currFrame.lastUsed();
        printf("   %u |%i  |%i  |%i \n", i, pgNum, pid, time);
    }
}

Frame &MemoryManager::getLru() {
    return _frames.at(_lruPosition);
}

