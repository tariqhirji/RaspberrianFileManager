// Utility that moves and renames files. Gives 2 parameters, one old name and one new name.
// Calls rename() function from fileManager on the given strings
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
        std::cout << "Error: Must call mymv with 2 strings" << std::endl;
    } else {
        // Call rewrite function to change name from old name to new name.
        // Save argv as string names, create fileManage and rewrite
        std::string oldName = argv[1];
        std::string newName = argv[2];
        fileManager fm (oldName);
        fm.rewrite(newName);
    }
}

