
/*

Backup.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/07/13 Original by Bowen Jin

--------------------------------------------------------------------------------------------------------------------

This header file defines a namespace "Backup" that encapsulates functions necessary for performing backup operations. It includes declarations for initializing the backup process, executing the backup, and exiting the backup operation. The purpose of this header file is to provide an interface for managing backup tasks in a system, ensuring that necessary files are opened, the backup is created, and resources are properly closed after the operation. There are no parameters or return values for the functions defined, and all functions are declared as "static", indicating they are intended for use within the compilation unit that includes this header file.

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef BACKUP_H
#define BACKUP_H

#include "FileNotOpenException.h"
#include "FileOpenFailedException.h"
#include "RecordNotFoundException.h"

#include <fstream>

namespace Backup {

// --------------------------------------------------------------------------------------------------------------------

    /* Opens the necessary files for reading. Assumes all files exist and can be opened.

    Parameters: None

    Return: Does not return anything.
    */
    static void initBackup();

// --------------------------------------------------------------------------------------------------------------------

    /* Creates a backup of all open files. This function will copy each file to a binary file by using backupFile()

    Parameters: None

    Return: Does not return anything.
    */
    static void backup();

// --------------------------------------------------------------------------------------------------------------------

    /* Creates a backup of a specific open file. This function will copy the file to a binary file.

    Parameters: None

    Return: Does not return anything.
    */

    static void backSingleFile(const string& backupFileName, std::fstream& originalFile);

// --------------------------------------------------------------------------------------------------------------------

    /* Exits the backup operation and closes any open files or resources.

    Parameters: None

    Return: Does not return anything.
    */
    static void exitBackup();
}

#endif