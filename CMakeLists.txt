cmake_minimum_required(VERSION 3.13)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

#add_executable(runMain main.cpp)

add_executable(untitled main.cpp Process.cpp Process.h OsSimulation.cpp OsSimulation.h Disk.cpp Disk.h CPU.cpp CPU.h ProcState.h HelperFunctions.h HelperFunctions.cpp)