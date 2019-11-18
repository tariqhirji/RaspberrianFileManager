// Utility that removes a file if it is found. If file not found, return error
// Can pass multiple file names to argv[] and it will loop through them all
// Author: Tariq Hirji

#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include "fileManager.h"

// Main method that calls fileManager remove() function to remove file arguments given
int main(int argc, char *argv[]) {
    // Check to see that there are files given to remove
    if (argc == 1 ) { 
        std::cout << "Must call myrm with at least one file name to remove" << std::endl;
    } else {
        // For every parameter (starting at 1 since myrm is [0], and go to i<= argc - 1 to manage index)
        // Create file Manager for argv string and then call remove function
        for (int i = 1; i <= argc - 1; i++) {
            std::string fileName = argv[i];
            fileManager *fm = new fileManager(fileName);
            fm->remove();
        }
    }
}
