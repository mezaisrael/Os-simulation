//
// Created by israel on 3/28/19.
//

#include "OsSimulation.h"

//constructor
OsSimulation::OsSimulation() : pIdAvailable(rootPid) {
    int memory;
    int pages;
    int diskCount;
    std::cout << "Memory:(bytes)";
    std::cin >> memory;
    std::cout << "\npages:" << std::endl;
    std::cin >> pages;
    std::cout << "\nDisk:" << std::endl;
    std::cin >> diskCount;

    //create the root process
    Process rootProcess(pIdAvailable);

    //add the root process to the list of process control blocks
    processes.insert({rootProcess.getId(), rootProcess});
    //cpu starts running process with pid of 1
    cpu.run(rootProcess);

    disk.resize(diskCount);
}

void OsSimulation::promptForCommand() {
    std::string userInput;
    //flush cin buffer
    std::cin.ignore();

    while (userInput != "end") {
        std::cout << ">> ";

        getline(std::cin, userInput);

        if (userInput == "A") {
            startNewProcess();
        } else if (userInput == "S r") {
            snapShotReadyQueue();
        } else if (userInput == "Q") {
            rotateProcess();
        } else if (userInput == "fork") {
            fork(runningProcess());
        } else if (userInput == "wait") {
            waitForChildren();
        }else if (userInput == "info") {
            printProcessInfo();
        }else if (userInput == "exit") {
            //todo. running process terminates
//            exitProcess(&cpu.getRunning());
        } else {
            std::cout << "invalid input" << std::endl;
        }
    }

}

void OsSimulation::rotateProcess() {
    //move id of running process to back of readyQueue
    readyQueue.push_back(runningProcess().getId());
    //change the state of the running process to ready
    runningProcess().setState(ready);

    //get reference to the front of readyQueue
    Process& frontOfQ = processes.at(readyQueue.front());
    readyQueue.pop_front();
    //make cpu run it
    cpu.run(frontOfQ);
}

//starts a new process which is essentially a fork of the root
void OsSimulation::startNewProcess() {
    Process & root = processes.at(rootPid);

    fork(root);
}

void OsSimulation::snapShotReadyQueue() const {
    std::cout << "cpu: pid " << cpu.getRunning() << std::endl;
    std::cout << "r-q: " << std::endl;

    for (auto id : readyQueue) {
        std::cout << id << std::endl;
    }

    std::cout << std::endl;
}

void OsSimulation::fork(Process &forkingProcess) {
    //make a child of the parent
    Process childProc(pIdAvailable, forkingProcess.getId(), ready);

    forkingProcess.addChild(childProc.getId());

    //insert it to the process container
    processes.insert({childProc.getId(), childProc});

    //insert id to the ready queue
    readyQueue.push_back(childProc.getId());
}

//running process waits until its children calls exit
//if it doesnt have children let user know
void OsSimulation::waitForChildren() {
    //check if it has children
    if(!runningProcess().isParent()) {
        std::cout << "cant perform command on childless process." << std::endl;
        return;
    }

    //add the running process to the waiting queue
    waitingQueue.push_back(runningProcess().getId());

    //get next in ready queue
    Process & nextInLine = processes.at(readyQueue.front());
    readyQueue.pop_front();
    //run the next in line
    cpu.run(nextInLine);
}

void OsSimulation::printProcessInfo() {
    int pId = pIdAvailable + 1;
    while (pId >= pIdAvailable && pId > 0) {
        std::cout << "enter pid: ";
        std::cin >> pId;
    }

    Process & process = processes.at(pId);
    std::cout << "pid     : " << process.getId() << std::endl;
    std::cout << "parent  : " << process.getParent() << std::endl;
    std::cout << "children: " << std::endl;
    std::vector<int>& children = process.getChildren();
    for (auto childId : children) {
        std::cout << childId << std::endl;
    }
}

void OsSimulation::exitProcess(Process* processPtr) {
//    std::vector<Process*> &children = cpu.getRunning().getChildren();
//
//    //if doesnt have any children just delete it
//    //from the process
//    if (!processPtr->isParent()) {
//        removeFromSystem(processPtr->getId());
//    }
//
//    for(auto & childPtr : children) {
//        exitProcess(childPtr);
//    }
}

void OsSimulation::removeFromSystem(int pId) {
//    //get the state of the process
//    State stateOfProcess = processes.at(pid).getState();
//
//    switch (stateOfProcess) {
//        case(ready) :
//            //remove from ready deque
//            for(int i = 0; i < readyQueue.size(); i++) {
//                if (readyQueue.at(i) == pId) {
//                    readyQueue.erase(readyQueue.begin()+i);
//                }
//            }
//            break;
//        case (waiting) : //remove from waiting
//        break;
//        case(running) : //stop cpu from running it
//    }
//
//    //remove from processes
//    processes.erase(pId);

}

Process &OsSimulation::runningProcess() {
    return processes.at(cpu.getRunning());
}



