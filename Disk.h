//
// Created by israel on 3/28/19.
//

#ifndef UNTITLED_DISK_H
#define UNTITLED_DISK_H


#include <deque>

class Disk {
public:
    Disk();

    //pId attempts to use the disk if it busy return false
    //and puts it in the queue. Returns true if it was successful
    bool use(int pId);

    //the hard disk finished the work of the current process
    void finishJob();

    //return id of the current process using the disk
    int getProcess();
private:

    //pid of the current process using the disk
    //if 0 then it is idle
    int usingProcess;

    //queue of processes waiting to use the disk
    std::deque<int> waiting;
};


#endif //UNTITLED_DISK_H
