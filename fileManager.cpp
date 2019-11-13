// File Manager helper class that will be used by utility main functions.
// View header file for function prototypes and variable declarations
// Author: Tariq Hirji (250901578)

#include "fileManager.h"
using namespace std;

fileManager::fileManager(string file) {
    // Save stat structure to help initialize variables
    struct stat buff;
    stat(file.c_str(), &buff); // use "c_str()" function to pass the variable name as a string
    fileName = file;
    // Switch case to figure out what type of file is being referenced
    // MUST have 'break' after each case
    switch (buff.st_mode & S_IFMT) {
        case S_IFBLK:  
            fileType = "block device";           
            break;
        case S_IFCHR:  
            fileType = "character device";        
            break;
        case S_IFDIR:  
            fileType = "directory";               
            break;
        case S_IFIFO:  
            fileType = "FIFO/pipe";               
            break;
        case S_IFLNK:  
            fileType = "symbolic link";                 
            break;
        case S_IFREG:  
            fileType = "regular file";            
            break;
        case S_IFSOCK: 
            fileType = "socket";                 
            break;
        default:       
            fileType = "unknown";                
            break;
    }
    fileSize = buff.st_size;
    
    // Owner ID and user name
    ownerID = buff.st_uid;
    struct passwd *owner; // Create passwd pointer to hold owner information
    owner = getpwuid(ownerID); // Get passwd structure associated with ownerID value
    ownerName = owner->pw_name; // Access passwd ptr variable "pw_name" to get name of the owner

    // Group ID and group name
    groupID = buff.st_gid;
    struct group *grp; // Create group pointer to hold group information
    grp = getgrgid(groupID); // Get group structure associated with groupID value
    groupName = grp->gr_name; // Access group ptr variable "gr_name" to get name of the group

    // Store permissions as int and print string and int permission values
    permissions = buff.st_mode;

    // Get times from timespec structures of stat
    accessTime = buff.st_atime;
    modifyTime = buff.st_mtime;
    statusChangeTime = buff.st_ctime;
    blockSize = buff.st_blksize;
    errorNumber = 0;
}
