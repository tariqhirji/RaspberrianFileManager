// Utility that prints a file, or multiple files, to the terminal
// Can pass multiple file names to argv[]
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
    // If no arguments given, tell error to provide files
    if (argc == 1) {
        std::cout << "Please provide at least one file name for mycat" << std::endl;
    } else {
        // create output stream to dump too 
        std::ofstream buff;
        // For each value of argc, create fm object and dump to ofstream.
        for (int i = 1; i < argc; i++) {
            fileManager fm = fileManager(argv[i]);
            if (fm.getFileType() == "regular file") {
                fm.dump(buff);
                std::string newDumpee = argv[i];
                fileManager newDump = fileManager("dump_" + newDumpee);
                // Remove file created
                newDump.remove();
            }
        }
    }
}

