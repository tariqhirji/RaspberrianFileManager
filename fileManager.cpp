// File Manager helper class that will be used by utility main functions.
// View header file for function prototypes and variable declarations
// Author: Tariq Hirji (250901578)

#include "fileManager.h"
using namespace std;

// Constructor that initializes files variables and reads the stat() variables of  the file.
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

// Destuctor that does not need to "delete" any memory since "new" memory was not allocated
// Stack memory is popped from the stack as the function ends, so they will automatically be destroyed
fileManager::~fileManager() {}


// Function that takes a file stream as a parameter and copies the current file to the file stream
void fileManager::dump(ofstream &outStream) {
    // Check if file type is regular file. If not, print error and return
    if (this->getFileType() != "regular file") {
        cerr << "Can only dump from a regular file" << endl;
        return;
    } else {
        char dataToWrite[this->getBlockSize()];
        ifstream inFile;
        // Try opening the file and skip any KEEP whitespace found
        try {
            inFile.open(this->getFileName());
            inFile >> noskipws;

            outStream.open("dump_" + this->getFileName());
            // While not end of input file, pipe to char array to write
            while (!inFile.eof()) {
                inFile >> *dataToWrite;
                if (inFile.good()) {
                    cout << *dataToWrite;
                    outStream << *dataToWrite;
                }
            }
            // Close files
            outStream.close();
            inFile.close();
        } catch (exception e) {
            // If exception caught, print error and reclose files incase code not reached
            cout << "Error Dumping" << endl;
            outStream.close();
            inFile.close();
        }
    } 
}


// Function that rewrites the file name to the new given name. 
// *Changed from rename() since it uses a separate "rename()" function*
void fileManager::rewrite(string newFileName) {
    // Save old file name in character array to use in rename().
    char * oldName = new char[fileName.length() + 1];
    strcpy(oldName, fileName.c_str());
    
    // Save new file name in character array to use in rename().
    char * newName = new char[newFileName.length()+1];
    strcpy(newName, newFileName.c_str());

    // Set new file name 
    fileName = newFileName;

    // Test and call rename() on char []'s. If error encountered, print to console. 
    if (rename(oldName, newName) != 0) {
        perror("Error renaming file");
    }

    // Delete memory allocated for char arrays
    delete[] oldName;
    delete[] newName;
}

// Function that removes the current fileManager object from the file system.
// Uses unlink() and must check if unlink created an error or not
void fileManager::remove() {
    // Save file name in char array to be unlinked
    char * delFile = new char[fileName.length() + 1];
    strcpy(delFile, fileName.c_str());

    // Call unlink and then destructor
    int delTest = unlink(delFile);
    if (delTest == 0) {
        // Call destructor once unlinked
         // Clear memory allocated for char array
        delete[] delFile;
    } else {
        // If not 0, error occured
        perror("Error deleting file");
    }
}


// Function that compares a given file with the current file
// CANNOT compare directories, must be files.
// Read until eof and check is similar. Print statement is similar or not
void fileManager::compare(fileManager *checkFile) {

    // If either file is a directory, cannot compare. Return out
    if (this->getFileType() == "directory" || checkFile->getFileType() == "directory") {
        cout << "Cannot compare directories" << endl;
        return;
    }

    // Create char arrays for file streams to add chars to.
    char currentFile[this->getBlockSize()];
    char newFile[checkFile->getBlockSize()];

    // Create file streams that open the checkFile and current file
    ifstream currentIFS (this->getFileName(), ifstream::in);
    ifstream newIFS (checkFile->getFileName(), ifstream::in);

    int outcome = 1;
    // While not the end of either file stream, add chars and then compare strings
    // If strings are ever not equal, break and print wrong message
    while (!currentIFS.eof() || !newIFS.eof()) {
        currentIFS >> currentFile;
        newIFS >> newFile;
        // If characters are not same in char arrays, files are different
        if (*currentFile != *newFile) {
            outcome = -1;
            break;
        } else {
            outcome = 0;
        }
    }
    // Check outcome variable to see if the files are the same or different
    if (outcome == 0) {
        cout << "The files are the same!" << endl;
    } else {
        cout << "The files are NOT the same." <<endl;
    }
}

// Function that expands the current directory and defines it's children files
// If current file type is not directory, return error
void fileManager::expand() {
    // Check to see if file type is directory
    // If directory, perform functions, else then print to error stream
    if (this->getFileType() == "directory") {
        // Create directory stream and check that no errors occured
        DIR *dirStream = opendir(this->getFileName().c_str());
        if (dirStream == NULL) {
            cerr << "Error number: " << this->getErrorNumber() << endl;
            perror("Error opening direcotry Stream");
            abort();
        }
        // If directory stream is not null, get direct struct
        struct dirent *dirSpot;
        // while the directory stream pointer is no pointing to a null point
        // then make FM class for each subfile and push onto child vector               
        while ((dirSpot = readdir(dirStream))) {
            string newFile = dirSpot->d_name;
            fileManager *subFile = new fileManager(newFile);
            // cout << subFile->getFileName() << endl;
            children.push_back(*subFile);
            // Delete memory used for subfile after pushed into vector
            delete subFile;
        }
        // Close the directory Stream and check if error occured when closing.
        int close = closedir(dirStream);
        if (close == -1) {
            cerr << "Error Number: " << this->getErrorNumber() << endl;
            perror("Encountered the following issue");
            abort();
        } else {
            return;
        }
    } else {
        cerr << "given string is not a directory or could not be found" << endl;
    }
}

// Getter method that returns the file name
string fileManager::getFileName() {
    return fileName;
}

// Getter method that returns the file type
string fileManager::getFileType() {
    return fileType;
}

// Getter method that returns the file size (bytes)
off_t fileManager::getFileSize() {
    return fileSize;
}

// Getter method that returns the owner's ID
uid_t fileManager::getOwnerID(){
    return ownerID;
}

// Getter method that returns the owner's name
string fileManager::getOwnerName() {
    return ownerName;
}

// Getter method that returns the group's ID 
gid_t fileManager::getGroupID() {
    return groupID;
}

// Getter method that returns the group's name
string fileManager::getGroupName() {
    return groupName;
}

// Getter method that returns both the string and the int permissions
void fileManager::getPermissions(){
    struct stat buff;
    stat(fileName.c_str(), &buff);
    permissions = buff.st_mode;

    // Print the permissions inline based on the flag bit for permissions
    cout << (S_ISDIR(buff.st_mode) ? "d" : "-");
    cout << ((buff.st_mode & S_IRUSR) ? "r" : "-");
    cout << ((buff.st_mode & S_IWUSR) ? "w" : "-");
    cout << ((buff.st_mode & S_IXUSR) ? "x" : "-");
    cout << ((buff.st_mode & S_IRGRP) ? "r" : "-");
    cout << ((buff.st_mode & S_IWGRP) ? "w" : "-");
    cout << ((buff.st_mode & S_IXGRP) ? "x" : "-");
    cout << ((buff.st_mode & S_IROTH) ? "r" : "-");
    cout << ((buff.st_mode & S_IWOTH) ? "w" : "-");
    cout << ((buff.st_mode & S_IXOTH) ? "x" : "-");
}

// Getter method that converts time_t variable to date and returns the last accessed time/date
string fileManager::getAccessTime(){
    time_t rawtime = accessTime;
    struct tm * accessInfo;
    accessInfo = localtime(&rawtime);
    return asctime(accessInfo);
}

// Getter method that converts time_t variable to date and returns the last modified time/date
string fileManager::getModifyTime() {
    time_t rawtime = modifyTime;
    struct tm * modifyInfo;
    modifyInfo = localtime(&rawtime);
    return asctime(modifyInfo);
}

// Getter method that converts time_t variable to date and returns the last status change time/date
string fileManager::getStatusTime() {
    time_t rawtime = statusChangeTime;
    struct tm * statusInfo;
    statusInfo = localtime(&rawtime);
    return asctime(statusInfo);
}

// Getter method that returns the block size
blksize_t fileManager::getBlockSize() {
    return blockSize;
}

// Getter method that returns the children vector of the file
vector<fileManager> fileManager::getChildren() {
    return children;
}

// Getter method that returns the error number whenever it is adjusted.
int fileManager::getErrorNumber() {
    return errorNumber;
}