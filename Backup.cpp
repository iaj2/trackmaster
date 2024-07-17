// Backup.cpp

#include "Backup.h"
#include <iostream>
#include <fstream>

using namespace std;

static fstream backupFile;

static fstream changeFile;
static fstream productFile;
static fstream productReleaseFile;
static fstream requestFile;
static fstream requesterFile;


void Backup::initBackup() {

    changeFile.open("request.bin", ios::in | ios::binary);
    productFile.open("product.bin", ios::in | ios::binary);
    productReleaseFile.open("productRelease.bin", ios::in | ios::binary);
    requestFile.open("request.bin", ios::in | ios::binary);
    requesterFile.open("requester.bin", ios::in | ios::binary);

}



static void Backup::backup() {

    // Backup changeFile
    backSingleFile("changeFile.dat", changeFile);

    // Backup productFile
    backSingleFile("productFile.dat", productFile);

    // Backup productReleaseFile
    backSingleFile("productReleaseFile.dat", productReleaseFile);

    // Backup requestFile
    backSingleFile("requestFile.dat", requestFile);

    // Backup requesterFile
    backSingleFile("requesterFile.dat", requesterFile);
}


// Helper function for backup(). Assumes all "originalFile" files are opened. 
void Backup::backSingleFile(const string& backupFileName, fstream& originalFile) {

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


void Backup::exitBackup() {
    // Exits the backup operation and closes any open files or resources.
    if (backupFile.is_open()) {
        backupFile.close();
    }
}
