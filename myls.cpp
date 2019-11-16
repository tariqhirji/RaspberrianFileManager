// Utility that lists a directories contents or a files name.
// Can be given no parameters and print current directory, or a directory or a file parameter can be given.
// Can also be given '-l' flag to list more detail about files.
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
    // First case: No parameter given
    if (argc == 1) {
        std::string currentFile = ".";
        fileManager fm = fileManager(currentFile);
        fm.expand();
        std::vector<fileManager> kids = fm.getChildren();
        for (int i =0; i < kids.size(); i++) {
            std::cout << kids[i].getFileName() << std::endl;
        }
    }
    // Second case: a parameter given (could be a "-l")
    else if (argc == 2) {
        std::string fileName = argv[1];
        // Compare string to see if "-l" flag
        int test = std::strcmp(fileName.c_str(), "-l");
        // If parameter is "-l", print children data
        if (test == 0) {
            // Set file name to current directory
            fileName = ".";
            fileManager fm = fileManager(fileName);
            fm.expand();
            // Print data for each child
            std::vector<fileManager> kids = fm.getChildren();
            for (int i = 0; i < kids.size(); i++) {
                std::cout << kids[i].getFileName() << ' ';
                std::cout << kids[i].getOwnerID() << ' ';
                std::cout << kids[i].getOwnerName() << ' ';
                std::cout << kids[i].getAccessTime() << ' ';
                kids[i].getPermissions();
                std::cout << '\n' << std::endl;
            }
        } else {
            // If test is not equal, check file to see if directory
            fileManager fm = fileManager(fileName);
            if (fm.getFileType() == "directory") {
                fm.expand();
                std::vector<fileManager> kids = fm.getChildren();
                for (int i =0; i < kids.size(); i++) {
                    std::cout << kids[i].getFileName() << std::endl;
                }
            } else {
                // If not directory, just print file name
                std::cout << fm.getFileName() << std::endl; 
            }
        }
    } else {
        std::cout << "Must give myls either no parameters, a directory name, a file name or a '-l' flag" << std::endl;
    }
}
