//
// Created by israel on 3/28/19.
//

#include <sstream>
#include <zconf.h>
#include "OsSimulation.h"
#include "HelperFunctions.h"

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
    //flush cin
    std::cin.ignore();

    //create the root process
    Process rootProcess(pIdAvailable);

    //add the root process to the list of process control blocks
    processes.insert({rootProcess.getId(), rootProcess});
    //cpu starts running process with pid of 1
    cpu.run(rootProcess);

    disks.resize(diskCount);
}

void OsSimulation::promptForCommand() {
    std::string userInput;

    while (userInput != "end") {
        std::cout << ">> ";
        getline(std::cin, userInput);

        if (userInput == "") {
            continue;
        }
        
        //split the string to get inputs with numbers;
        std::vector<std::string> commands = split(userInput);

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
        } else if (commands.at(0) == "d") {
            try {
                int diskNum = stoi(commands.at(1));
                //the diskNUm and the fileName
                requestFile(diskNum, commands.at(2));
            } catch(...) {
                std::cout << "Input is not a number" << std::endl;
            }
        } else if (commands.at(0) == "D") {
            try {
                int diskNum = stoi(commands.at(1));
                diskFinish(diskNum);
            } catch (...) {
                std::cout << "Input is not a number" << std::endl;
            }
        } else if(userInput == "S i") {
            snapShotIO();
        } else if (userInput == "info") {
            printProcessInfo();
        } else if (userInput == "exit") {
            //todo. test this function
            exitRunning();
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
        std::cout << "  enter pid: ";
        std::cin >> pId;
        std::cin.ignore();
    }

    Process & process = processes.at(pId);
    std::cout << "pid     : " << process.getId() << std::endl;
    std::cout << "parent  : " << process.getParent() << std::endl;
    std::cout << "file    : " << process.getFile() << std::endl;
    std::cout << "children: ";
    std::unordered_set<int>& children = process.getChildren();
    for (auto childId : children) {
        std::cout << childId << ", " << std::endl;
    }
}

void OsSimulation::exitRunning() {
    //if parent is waiting make it ready
    int parentId = runningProcess().getParent();
    if(parentId != 0 && processes.at(parentId).getState() == waiting) {
        processes.at(parentId).setState(ready);
        readyQueue.push_back(runningProcess().getId());
        endProcess(runningProcess());
    } else {
        //if parent hasn't called wait, make it and its decedents zombies
        turnToZombie(runningProcess());
    }

    //run next in ready queue
    cpu.run(processes.at(readyQueue.front()));
    readyQueue.pop_front();
}

void OsSimulation::endProcess(Process & process) {
    //base case
    if (!process.isParent()) {
        //if its in ready que remove it
        //todo: if its in disk, remove from disk once implemented
        if (process.getState() == ready) {
        } else if(process.getState() == waiting) {

        }
        processes.erase(process.getId());
    }

    std::unordered_set<int> childrenIds = process.getChildren();
    for(auto childId : childrenIds) {
        endProcess(processes.at(childId));
    }
}

void OsSimulation::turnToZombie(Process &infectedProcess) {

    switch (infectedProcess.getState()) {
        case ready :
            for (int i = 0; i < readyQueue.size(); ++i) {
                if(readyQueue.at(i) == infectedProcess.getId()) {
                    readyQueue.erase(readyQueue.begin()+i);
                }
            }
            break;
        case readyIO:
            //TODO remove from readyIo queue
            break;
        case usingDisk:
            //if it is using disk make disk run next in queue
            break;

    }

    infectedProcess.setState(terminated);
}

Process &OsSimulation::runningProcess() {
    return processes.at(cpu.getRunning());
}

void OsSimulation::requestFile(int dIndex, std::string &fileName) {
    if (dIndex >= disks.size()) {
        std::cout <<
            "system does not have "
            "that many Disk" << std::endl;
        return;
    }

    runningProcess().requestDisk(disks.at(dIndex), fileName);

    runNextInQueue();
}

void OsSimulation::diskFinish(int diskNum) {
    //get the id of the process using the disk
    int pidInDisk = disks.at(diskNum).getProcess();

    processes.at(pidInDisk).finishUsingDisk();
    disks.at(diskNum).finishJob();
    //add it to the readyQueue
    readyQueue.push_back(pidInDisk);
}

void OsSimulation::snapShotIO() {
    //iterate through all the disks
    std::cout << "inside snap shot" << std::endl;
    for (int i = 0; i < disks.size(); i++) {
        Disk &currentDisk = disks.at(i);

        if (currentDisk.isIdle()){
            std::cout << "Disk " << i << " idle" << std::endl;
            continue;
        }
        
        int pidInDisk = currentDisk.getProcess();

        std::cout << "Disk " << i << "using: "
        << "pId: " << pidInDisk
        << " file: " << processes.at(pidInDisk).getFile() << std::endl;

        std::cout << "  I/O Queue: " << std::endl;

        for (auto queueId : currentDisk.getQueue()) {
            Process &proInQueue = processes.at(queueId);
            std::cout << "  pid: "
            << proInQueue.getId() << " file:"
            << proInQueue.getFile() << std::endl;
        }
    }
}

void OsSimulation::runNextInQueue() {
    //run the nextInline
    int nextInline = readyQueue.front();
    readyQueue.pop_front();

    cpu.run(processes.at(nextInline));
}

