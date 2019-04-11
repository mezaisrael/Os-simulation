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
    pIdAvailable++;

    //add the root process to the list of process control blocks
    processes.insert({rootProcess.getId(), rootProcess});
    //cpu starts running process with pid of 1
    cpu.run(&processes.at(rootPid));

    disk.resize(diskCount);
}

void OsSimulation::promptForCommand() {
    std::string userInput;
    //flush cin buffer
    std::cin.ignore();

    while (userInput != "exit") {
        std::cout << ">> ";

        getline(std::cin, userInput);

        if (userInput == "A") {
            startNewProcess();
        } else if (userInput == "S r") {
            snapShotReadyQueue();
        } else if (userInput == "Q") {
            rotateProcess();
        } else if (userInput == "fork") {
            fork(cpu.getRunning());
        } else if (userInput == "wait") {
            waitForChildren();
        }else if (userInput == "info") {
            printProcessInfo();
        }else if (userInput == "exit") {
            //todo. running process terminates
            exitProcess();
        } else {
            std::cout << "invalid input" << std::endl;
        }
    }

}

void OsSimulation::rotateProcess() {
    //move id running process to back of readyQueue
    readyQueue.push(cpu.getRunning().getId());

    //get reference to the front of readyQueue
    Process &frontOfQ = processes.at(readyQueue.front());
    //make cpu run it (point to it)
    cpu.run(&frontOfQ);

    readyQueue.pop();
}

//starts a new process which is essentially a fork of the root
void OsSimulation::startNewProcess() {
    Process & root = processes.at(rootPid);

    fork(root);
}

void OsSimulation::snapShotReadyQueue() const {
    std::cout << "cpu: pid " << cpu.getRunning().getId() << std::endl;
    std::cout << "r-q: " << std::endl;

    std::queue<int> readyQCopy = readyQueue;

    while (!readyQCopy.empty()) {
        std::cout << "  pid " << readyQCopy.front() << std::endl;
        readyQCopy.pop();
    }

    std::cout << std::endl;
}

void OsSimulation::fork(Process &forkingProcess) {
    //pointer to child process
    Process * childProcPtr = new Process(pIdAvailable, forkingProcess.getId());
    pIdAvailable++;

    forkingProcess.addChild(childProcPtr);

    //insert it to the process container
    processes.insert({childProcPtr->getId(), *childProcPtr});

    //insert id to the ready queue
    readyQueue.push(childProcPtr->getId());
}

//running process waits until its children calls exit
//if it doesnt have children let user know
void OsSimulation::waitForChildren() {
    Process running = cpu.getRunning();

    //check if it has children
    if(running.getChildren().empty()) {
        std::cout << "cant perform command on childless process." << std::endl;
        return;
    }

    //add the running process to the waiting queue
    waitingQueue.push(running.getId());

    //get next in ready queue
    Process & nextInLine = processes.at(readyQueue.front());
    readyQueue.pop();
    //run the next in line
    cpu.run(&nextInLine);
}

void OsSimulation::printProcessInfo() {
    int pId = pIdAvailable + 1;
    while (pId >= pIdAvailable) {
        std::cout << "enter pid: ";
        std::cin >> pId;
    }

    Process & process = processes.at(pId);
    std::cout << "pid     : " << process.getId() << std::endl;
    std::cout << "parent  : " << process.getParent() << std::endl;
    std::cout << "children: " << std::endl;
    std::vector<Process *> children = process.getChildren();
    Process * currentChild = nullptr;
    for(int i = 0; i < children.size(); i++) {
        currentChild = children.at(i);
        std::cout << "--  " << currentChild->getId() << " " << std::endl;
    }
}

void OsSimulation::exitProcess() {
//    if(!cpu.getRunning().getChildren().empty()) {
//        Process *currProcessPtr = nullptr;
//        for(int i = 0; i < currProcessPtr) {
//
//        }
//    }
}



