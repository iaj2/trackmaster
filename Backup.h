
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

#include <vector>
#include <string>

namespace Backup {

// --------------------------------------------------------------------------------------------------------------------

    /* Opens the necessary files and prepares the system for the backup operation.

    Parameters: In parameter.

    Return: Does not return anything.
    */
    void initBackup(const std::vector<std::string>& fileNames);

// --------------------------------------------------------------------------------------------------------------------

    /* Creates a backup of all open files. This function will copy each file to a binary file by using backupFile()

    Parameter: In parameter.

    Return: Does not return anything.
    */
    void backup(const std::vector<std::string>& fileNames);

// --------------------------------------------------------------------------------------------------------------------

    /* Exits the backup operation and closes any open files or resources.

    Parameters: In parameter.

    Return: Does not return anything.
    */
    void exitBackup(const std::vector<std::string>& fileNames);
}

#endif