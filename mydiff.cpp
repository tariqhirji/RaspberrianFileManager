// Utility that compares the given 2 parameters
// Calls compare() function from fileManager on the given strings
// Author: Tariq Hirji

#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include "fileManager.h"

// Main function that checks if the correct number of arguments are passed
int main(int argc, char *argv[]) {
    // Check to see if enough parameters are given. If not, print error
    if (argc != 3) {
        std::cout << "Error: Must call mydiff with 2 strings" << std::endl;
    } else {
        // Save argv as string names, create fileManager objects for each argv and compare to each other
        std::string firstFile = argv[1];
        std::string secondFile = argv[2];
        fileManager fm (firstFile);
        fileManager *fm2 = new fileManager(secondFile);
        fm.compare(fm2);
    }
}
