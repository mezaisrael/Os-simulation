//
// Created by israel on 3/29/19.
//

#include "CPU.h"

void CPU::run(Process &newRunning) {
    processingId = newRunning.getId();
    if (newRunning.getId() != 1) {
        newRunning.setState(running);
    }
}

int CPU::getRunning() const{
    return processingId;
}

bool CPU::isIdle() const {
    return (getRunning() == 1);
}

