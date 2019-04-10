//
// Created by israel on 3/29/19.
//

#ifndef UNTITLED_CPU_H
#define UNTITLED_CPU_H


#include "Process.h"

class CPU {
public:
    //pass it a new Process to process
    void run(Process *newRunning);

    //returns reference to running process
    Process getRunning() const;
private:
    Process *currentRunningProc;
};


#endif //UNTITLED_CPU_H
