// Utility that copies a file to a new spot. Take an old name and make it a new name.
// Passes two parameters, oldName and newName
// Author: Tariq Hirji


#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include "fileManager.h"

// Main method that calls fileManager dump() function to move file arguments.
int main(int argc, char *argv[]) {
    // Check to see that there are files given to remove
    if (argc != 3 ) { 
        std::cout << "Must call mycp with 2 file names" << std::endl;
    } else {
        // Store parameters in strings and make fm object for old name
        std::string source = argv[1];
        std::string destination = argv[2];
        fileManager fm = fileManager(source);
        // Make ofstream to pass to dump to copy 
        // also rename the new dumped file to change copied file name
        std::ofstream buff;
        fm.dump(buff);
        fileManager fm2 = fileManager("dump_" + source);
        fm2.rewrite(destination);
    }
}
