// File Manager header class that outlines the utilities used to manage the file system
// Includes a lot of intimidating types but all the types can be defined in the library and based on the stat() function.
// Author: Tariq Hirji

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


class fileManager {
    public:
        fileManager(std::string file); //ctor
        ~fileManager(); //dtor
        void dump(std::ofstream &outStream);  // util
        void rewrite(std::string newFileName); // util
        void remove(); // util
        void compare(fileManager *checkFile); // util
        void expand(); // util
        // Getter methods
        std::string getFileName();
        std::string getFileType();
        off_t getFileSize();
        uid_t getOwnerID();
        std::string getOwnerName();
        gid_t getGroupID();
        std::string getGroupName();
        void getPermissions();
        std::string getAccessTime();
        std::string getModifyTime();
        std::string getStatusTime();
        blksize_t getBlockSize();
        std::vector<fileManager> getChildren();
        int getErrorNumber();
    protected:
    private:
        std::string fileName;
        std::string fileType; // Separated into string in ctor
        off_t fileSize;
        uid_t ownerID;
        std::string ownerName;
        gid_t groupID;
        std::string groupName;
        int permissions;
        time_t accessTime; // seconds converted to date
        time_t modifyTime; // seconds converted to date
        time_t statusChangeTime; // seconds converted to date
        blksize_t blockSize;
        std::vector<fileManager> children; // Vector of fileManagers to hold all inner files (if directory)
        int errorNumber;
};

#endif /* FILEMANAGER_H */

