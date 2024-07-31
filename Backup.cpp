/*
Backup.cpp
--------------------------------------------------------------------------------------------------------------------
Revision History:
Rev. 1 - 2024/07/13 Original by Bowen Jin
Rev. 2 - 2024/07/30 
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related Backup.h
--------------------------------------------------------------------------------------------------------------------
*/

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string.h>
#include <vector>
#include <filesystem>

#include "Backup.h"

namespace fs = std::filesystem;

namespace Backup{
    void initBackup(const std::vector<std::string>& fileNames) {
    for (const auto& fileName : fileNames) {
        if (!fs::exists(fileName)) {
            throw std::runtime_error("File does not exist: " + fileName);
        }
        std::ifstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + fileName);
            }
        }
    }

    void backup(const std::vector<std::string>& fileNames) {
        for (const auto& fileName : fileNames) {
            std::string backupFileName = "backup_" + fileName;
            fs::copy_file(fileName, backupFileName, fs::copy_options::overwrite_existing);
        }
    }


    void exitBackup(const std::vector<std::string>& fileNames) {
        for (const auto& fileName : fileNames) {
            std::ifstream file(fileName);
            if (file.is_open()) {
                file.close();
            }
        }
    }

}

