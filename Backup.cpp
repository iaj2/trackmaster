// Backup.cpp

#include "Backup.h"
#include <iostream>
#include <fstream>

using namespace std;

static fstream backupFile;


// Checks if all files that need to be backed up are open
static void Backup::initBackup() {

    if (!changeFile.is_open()) {
        throw FileNotOpenException("File is not open");
    }


    if (!productFile.is_open()) {
        throw FileNotOpenException("File is not open");
    }


    if (!productReleaseFile.is_open()) {
        throw FileNotOpenException("File is not open");
    }

    if (!requestFile.is_open()) {
        throw FileNotOpenException("File is not open");
    }

    if (!requesterFile.is_open()) {
        throw FileNotOpenException("File is not open");
    }
}



static void Backup::backup() {

    // Backup changeFile
    backupFile("changeFile.bin", changeFile);

    // Backup productFile
    backupFile("productFile.bin", productFile);

    // Backup productReleaseFile
    backupFile("productReleaseFile.bin", productReleaseFile);

    // Backup requestFile
    backupFile("requestFile.bin", requestFile);

    // Backup requesterFile
    backupFile("requesterFile.bin", requesterFile);

}


// Helper function for backup(). Assumes all "originalFile" files are opened. 
static void Backup::backupFile(const string& backupFileName, fstream& originalFile) {

    // If backupFileName already exists, clear. If not, create new.
    ofstream backup(backupFileName, ios::trunc | ios::binary);

    // Case 1: backupFileName cannot be opened.
    if (!backup.is_open()) {
        throw FileNotOpenException("File is not open");
    }

    // Copy contents from original file to backup file
    backup << originalFile.rdbuf();
    backup.close();
}


static void Backup::exitBackup() {
    // Exits the backup operation and closes any open files or resources.
    if (backupFile.is_open()) {
        backupFile.close();
    }
}
