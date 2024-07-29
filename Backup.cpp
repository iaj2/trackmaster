// Backup.cpp

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string.h>
#include <vector>
#include <filesystem>

#include "Backup.h"

namespace fs = std::filesystem;

void Backup::initBackup(const std::vector<std::string>& fileNames) {
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

void Backup::backup(const std::vector<std::string>& fileNames) {
    for (const auto& fileName : fileNames) {
        std::string backupFileName = "backup_" + fileName;
        fs::copy_file(fileName, backupFileName, fs::copy_options::overwrite_existing);
    }
}


 void Backup::exitBackup(const std::vector<std::string>& fileNames) {
    for (const auto& fileName : fileNames) {
        std::ifstream file(fileName);
        if (file.is_open()) {
            file.close();
        }
    }
}
