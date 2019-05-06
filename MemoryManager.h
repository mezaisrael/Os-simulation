//
// Created by israel on 5/2/19.
//

#ifndef UNTITLED_MEMORYMANAGER_H
#define UNTITLED_MEMORYMANAGER_H


#include <vector>
#include "Frame.h"

class MemoryManager {
public:
    MemoryManager();

    //set the memory, page size, and number of frames.
    void initiate(int memory, int pgSize);
    void request(int address, int pId);

    void snapshot();

    Frame & getLru();
private:
    int _time;
    int _memory;
    int _pageSize;
    std::vector<Frame> _frames;

    //hold the position of the of least used frame.
    // It can be calculated when checking for
    //if the pid is with given page# is in any frame
    int _lruPosition;
};


#endif //UNTITLED_MEMORYMANAGER_H
