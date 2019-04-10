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

    Process rootProcess(pIdAvailable);
    pIdAvailable++;
    //add the root process to the list of process control blocks
    pcbs.insert({rootProcess.getId(), rootProcess});
    //cpu starts running process with pid of 1
    cpu.run(&pcbs.at(rootPid));

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
            forkRunning();
        } else if (userInput == "exit") {
            std::cout << "exiting program" << std::endl;
        } else {
            std::cout << "invalid input" << std::endl;
        }
    }

}

void OsSimulation::rotateProcess() {
    //move id running process to back of readyQueue
    readyQueue.push(cpu.getRunning().getId());

    //get reference to the front of readyQueue
    Process &frontOfQ = pcbs.at(readyQueue.front());
    //make cpu run it (point to it)
    cpu.run(&frontOfQ);

    readyQueue.pop();
}

void OsSimulation::startNewProcess() {
    Process &root = pcbs.at(1);
    //fork return the child process created
    Process newProcess = root.fork(pIdAvailable);
    pcbs.insert({pIdAvailable, newProcess});

    //insert forked to the ready queue
    readyQueue.push(newProcess.getId());

    pIdAvailable++;
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

Process OsSimulation::forkRunning() {
    Process child = cpu.getRunning().fork(pIdAvailable);
    pIdAvailable++;

    //insert it to the process control blocks
    pcbs.insert({child.getId(), child});

    //the forked process is in the ready state
    readyQueue.push(child.getId());
    
    std::cout << "fork process. child id is " << child.getId()
    << "his parent is " << child.getParent() << std::endl;
}





