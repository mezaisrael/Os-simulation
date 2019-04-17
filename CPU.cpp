//
// Created by israel on 3/29/19.
//

#include "CPU.h"

void CPU::run(Process &newRunning) {
    processingId = newRunning.getId();
    newRunning.setState(running);
}

int CPU::getRunning() const {
    return processingId;
}
