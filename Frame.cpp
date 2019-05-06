//
// Created by israel on 5/2/19.
//

#include "Frame.h"

Frame::Frame() : _lastUsed(-1), _pid(0), _pageNumber(-1) {}

int Frame::lastUsed() const {
    return _lastUsed;
}

int Frame::getPid() const {
    return _pid;
}

int Frame::getPageNumber() const {
    return _pageNumber;
}

void Frame::setTime(int time) {
    _lastUsed = time;
}

void Frame::setFrame(int pid, int pageNum, int time) {
    _pid = pid;
    _pageNumber = pageNum;
    _lastUsed = time;
}

bool Frame::isEmpty() {
    return _pid == 0;
}


