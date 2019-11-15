// Utility that outputs the information for the given parameter. Gives only 1 parameter
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
    if (argc != 2) {
        std::cout << "Error: Must call mystat with 1 string" << std::endl;
    } else {
        fileManager fm = fileManager(argv[1]);
        
        std::cout << '\n';

        // Use getter functions to return variable fields
        std::cout << "File Name: " << fm.getFileName() << std::endl;
        std::cout << "File Type: " << fm.getFileType() << std::endl;
        std::cout << "File Size (Bytes): " << fm.getFileSize() << std::endl;

        // Spacing
        std::cout << '\n';

        std::cout << "Owner ID: " << fm.getOwnerID() << '\t';
        std::cout << "Owner Name: " << fm.getOwnerName() << std::endl;

        std::cout << "Group ID: " << fm.getGroupID() << '\t';
        std::cout << "Group Name: " << fm.getGroupName() << std::endl;

        // Spacing
        std::cout << '\n';

        std::cout << "Permissions: " << ' ';
        fm.getPermissions();
        
        // Spacing
        std::cout << '\n' << '\n';

        // Print children if file is a directory, otherwise provide text showing no Children
        std::cout << "Children: " << '\n';
        if (fm.getFileType() == "directory") {
            fm.expand();
        } else {
            std::cout << "Not applicable" << std::endl;
        }

        // Print file name of each children (if directory)
        std::vector<fileManager> kids = fm.getChildren();
        for (int i =0; i < kids.size(); i++) {
            std::cout << kids[i].getFileName() << std::endl;
        }

        // Spacing
        std::cout << '\n';

        std::cout << "Last access: " << fm.getAccessTime() << std::endl;
        std::cout << "Last modification: " << fm.getModifyTime() << std::endl;
        std::cout << "Last status change: " << fm.getStatusTime() << std::endl;

        std::cout << "Block size: " << fm.getBlockSize() << std::endl;

        std::cout << "Error Number: " << fm.getErrorNumber() << std::endl;
        
        std::cout << '\n';
    }
}

