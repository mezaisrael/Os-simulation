//
// Created by israel on 3/28/19.
//

#include <sstream>
#include "OsSimulation.h"
#include "HelperFunctions.h"

//constructor
OsSimulation::OsSimulation() : pIdAvailable(rootPid) {
    unsigned diskCount;
    unsigned int memory;
    unsigned pages;
    while (true) {
        std::cout << "Memory:(bytes)";
        std::cin >> memory;
        std::cout << "\npage size:" << std::endl;
        std::cin >> pages;
        std::cout << "\nDisk:" << std::endl;
        std::cin >> diskCount;

        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
        } else {
            break;
        }
    }

    //flush cin
    std::cin.ignore();

    //create the root process
    Process rootProcess(pIdAvailable);

    //add the root process to the list of process control
    processControl.insert({rootProcess.getId(), rootProcess});
    //cpu starts running process with pid of 1 which
    //means it is idle
    cpu.run(rootProcess);

    disks.resize(diskCount);

    _memoryManager.initiate(memory, pages);
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
                std::cout << "error" << std::endl;
            }
        } else if(userInput == "S i") {
            snapShotIO();
        } else if (commands.at(0) == "m") {
            try {
                int address = stoi(commands.at(1));
                requestMemory(address);
            } catch (...) {
                std::cout << "not a valid memory address" << std::endl;
            }

        }
        else if (userInput == "info") {
            printProcessInfo();
        } else if (userInput == "exit") {
            //todo. test this function
            exitRunning();
        } else if(userInput == "S m") {
            _memoryManager.snapshot();
        } else if(userInput == "end") {
            std::cout << "Good bye" << std::endl;
        }
        else {
            std::cout << "invalid input" << std::endl;
        }

        if (cpu.isIdle()) {
           runNextInQueue();
        }
    }

}

void OsSimulation::rotateProcess() {
    //move id of running process to back of readyQueue
    readyQueue.push_back(runningProcess().getId());
    //change the state of the running process to ready
    runningProcess().setState(ready);

    runNextInQueue();
}

//starts a new process which is essentially a fork of the root
void OsSimulation::startNewProcess() {
    Process & root = processControl.at(rootPid);

    fork(root);
}

void OsSimulation::snapShotReadyQueue() const {

    std::string running = cpu.isIdle() ? "idle" : std::to_string(cpu.getRunning());

    std::cout << "cpu pid|" << running << std::endl;
    printf("r_Q pid|");
    for (auto id : readyQueue) {
        printf("<-%i", id);
    }
    std::cout << std::endl;
}

void OsSimulation::fork(Process &forkingProcess) {
    //make a child of the parent
    Process childProc(pIdAvailable, forkingProcess.getId(), ready);

    forkingProcess.addChild(childProc.getId());

    //insert it to the process container
    processControl.insert({childProc.getId(), childProc});

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

    //check if any of its children are zombies
    //if so then consume it and no deed to wait
    for(auto childId : runningProcess().getChildren()) {
        Process& childProcess = processControl.at(childId);
        if (childProcess.getState() == terminated) {
            //remove the zombie process
            runningProcess().removeChild(childId);
            //and also remove it from the process control
            processControl.erase(childId);
            return;
        }
    }

    //add the running process to the waiting queue
    runningProcess().setState(waiting);

    runNextInQueue();
}

void OsSimulation::printProcessInfo() {
    int pId = pIdAvailable + 1;
    while (pId >= pIdAvailable && pId > 0) {
        std::cout << "  enter pid: ";
        std::cin >> pId;
        std::cin.ignore();
    }

    Process & process = processControl.at(pId);
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
    if (cpu.isIdle()) {
        std::cout << "No process to exit" << std::endl;
        return;
    }

    int runningId = cpu.getRunning();

    //free from memory
    if(_memoryManager.isInMem(runningId)) {
        _memoryManager.remove(runningId);
    }

    int parentId = runningProcess().getParent();
    Process &parentPro = processControl.at(parentId);
    //if parent is waiting make it ready only if it is it is not the root
    if(parentPro.getState() == waiting) {
        //if the parent is not the root, put it back in the ready state and in the ready queue
        if (parentPro.getId() != rootPid) {
            parentPro.setState(ready);
            readyQueue.push_back(parentPro.getId());
        }

        parentPro.removeChild(runningId);
        terminate(runningProcess());
    } else {
        //if parent hasn't called wait, make the running process a zombie(still in process control)
        runningProcess().setState(terminated);
        //terminate its children
        for (auto childId : runningProcess().getChildren()) {
            terminate(processControl.at(childId));
        }
    }

    runNextInQueue();
}

void OsSimulation::terminate(Process &process) {
    //free from memory
    if(_memoryManager.isInMem(process.getId())) {
        _memoryManager.remove(process.getId());
    }

    //base case: If process is childless
    if (!process.isParent()) {
        //if its in readyQueue remove it
        if (process.getState() == ready) {
            removeFromReady(process.getId());
        } else if(process.getState() == readyIO) {
            disks.at(process.getDisk()).remove(process.getId());
        } else if (process.getState() == usingDisk) {
            disks.at(process.getDisk()).finishJob();
        }

        //remove from process control
        processControl.erase(process.getId());
        return;
    }

    //recursively terminate its children
    std::unordered_set<int> childrenIds = process.getChildren();
    for(auto childId : childrenIds) {
        terminate(processControl.at(childId));
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
    return processControl.at(cpu.getRunning());
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
    //the disk who has completed its job
    Disk& myDisk = disks.at(diskNum);
    if (myDisk.isIdle()) {
        std::cout << "can finish job of empty disk" << std::endl;
        return;
    }
    //get the id of the process using the disk
    int pidInDisk = myDisk.getProcess();

    processControl.at(pidInDisk).finishUsingDisk();
    //add it to the readyQueue
    readyQueue.push_back(pidInDisk);

    //disk finished its job
    myDisk.finishJob();
    if (!myDisk.isIdle()) {
        int newInDisk = myDisk.getProcess();
        processControl.at(newInDisk).setState(usingDisk);
    }
}

void OsSimulation::snapShotIO() {
    //iterate through all the disks
    for (int i = 0; i < disks.size(); i++) {
        Disk &currentDisk = disks.at(i);

        if (currentDisk.isIdle()){
            std::cout << "Disk " << i << " idle" << std::endl;
            continue;
        }
        
        int pidInDisk = currentDisk.getProcess();

        printf("Disk %*d| pid | file \n", 3, i);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf(" in disk|%*d |",4, pidInDisk);
        std::string fileName = processControl.at(pidInDisk).getFile();
        std::cout << fileName << std::endl;
        for (auto queueId : currentDisk.getQueue()) {
            Process &proInQueue = processControl.at(queueId);
            printf ("  queue |%*d | ", 4, proInQueue.getId());
            std::cout << proInQueue.getFile() << std::endl;
        }
        std::cout << std::endl;
    }
}

void OsSimulation::runNextInQueue() {
    if (readyQueue.empty()) {
        //if cpu holds root than it is idle
        cpu.run(processControl.at(rootPid));
        return;
    }

    //run the nextInline
    int nextInline = readyQueue.front();
    readyQueue.pop_front();

    cpu.run(processControl.at(nextInline));
}

void OsSimulation::requestMemory(int address) {
    if (cpu.isIdle()) {
        std::cout << "cpu is idle" << std::endl;
        return;
    }

    _memoryManager.request(address, cpu.getRunning());
}

void OsSimulation::removeFromReady(int id) {
    for(auto it = readyQueue.begin(); it < readyQueue.end(); it++) {
        if (*it == id) {
            readyQueue.erase(it);
        }
    }
}

