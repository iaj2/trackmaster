// Backup.cpp

#include "Backup.h"

#include "Change.h"
#include "Product.h"
#include "ProductRelease.h"
#include "Request.h"
#include "Requester.h"


#include <iostream>
#include <fstream>

using namespace std;

static fstream backupFile;

fstream Change::changeFile;
fstream Product::productFile;
fstream ProductRelease::productReleaseFile;
fstream Request::requestFile;
fstream Requester::requesterFile;

// --------------------------------------------------------------------------------------------------------------------
/*

Function: initBackup()

Opening specific binary files for reading. These files will be read and its contents written on a separate .dat file.
This function assumes that all .bin files can and have been opened already.

*/

void Backup::initBackup() {

    changeFile.open("request.bin", ios::in | ios::binary);
    productFile.open("product.bin", ios::in | ios::binary);
    productReleaseFile.open("productRelease.bin", ios::in | ios::binary);
    requestFile.open("request.bin", ios::in | ios::binary);
    requesterFile.open("requester.bin", ios::in | ios::binary);

}

// --------------------------------------------------------------------------------------------------------------------
/*

Function: backup()

This function is responsible for backing up multiple specific files by calling the backSingleFile function for each file individually.

*/

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


// --------------------------------------------------------------------------------------------------------------------
/*

Function: backSingleFile()

Implements the backup of a single file. This is the helper function for backup() and assumes all "originalFile" files are opened

If the specified backupFileName already exists, clears its contents.
Otherwise, create a new file.
Copies contents from originalFile to the backup file using the stream buffer, then closes the backup file.


If unable to open backupFileName, throws a FileOpenFailedException .

*/

void Backup::backSingleFile(const string& backupFileName, fstream& originalFile) {

    // If backupFileName already exists, clear. If not, create new.
    ofstream backup(backupFileName, ios::trunc | ios::binary);

    // Case 1: backupFileName cannot be opened.
    if (!backup.is_open()) {
        throw FileOpenFailedException("File cannot be opened");
    }

    // Copy contents from original file to backup file
    backup << originalFile.rdbuf();
    backup.close();
}


// --------------------------------------------------------------------------------------------------------------------
/*

Function: exitBackup()

*/

void Backup::exitBackup() {
    // Exits the backup operation and closes any open files or resources.
    if (backupFile.is_open()) {
        backupFile.close();
    }
}
