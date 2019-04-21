//
// Created by israel on 4/20/19.
//

#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> split(const std::string & stringToSplit) {
    std::istringstream iss(stringToSplit);
    std::vector<std::string> results;

    std::string token;
    while(getline(iss,token,' ')) {
        results.push_back(token);
    }
    return results;
}