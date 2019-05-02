//
// Created by israel on 3/29/19.
//

#ifndef UNTITLED_CPU_H
#define UNTITLED_CPU_H


#include "Process.h"

class CPU {
public:
    //pass it a new Process to process
    void run(Process &process);

    //returns id of the running proceess
    int getRunning() const;

    bool isIdle();
private:
    int processingId;
};


#endif //UNTITLED_CPU_H
