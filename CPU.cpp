//
// Created by israel on 3/29/19.
//

#include "CPU.h"

void CPU::run(Process *newRunning) {
    currentRunningProc = newRunning;
}

Process CPU::getRunning() const {
    return *currentRunningProc;
}
