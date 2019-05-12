//
// Created by israel on 5/2/19.
//

#ifndef UNTITLED_MEMORYMANAGER_H
#define UNTITLED_MEMORYMANAGER_H


#include <vector>
#include <unordered_map>
#include "Frame.h"

class MemoryManager {
public:
    MemoryManager();

    //set the memory, page size, and number of frames.
    void initiate(int memory, int pgSize);
    void request(int address, int pId);

    void snapshot();

    Frame & getLru();

    //return if the pid is in memory
    bool isInMem(int pid);

    //remove pid from memory
    void remove(int pid);
private:
    int _time;
    unsigned int _memory;
    unsigned int _pageSize;
    std::vector<Frame> _frames;

    //map pid to frame position
    std::unordered_map<int,int> _pidsInMem;

    //hold the position of the of least used frame.
    // It can be calculated when checking for
    //if the pid is with given page# is in any frame
    int _lruPosition;
};


#endif //UNTITLED_MEMORYMANAGER_H
