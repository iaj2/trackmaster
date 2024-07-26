#include "EntityIO.h"
#include <iostream>

template<typename T>
EntityIO<T>::EntityIO(const std::string& filename) : filename(filename), recordCount(0) {}

template<typename T>
void EntityIO<T>::init() {
    // Attempt to open the file in read/write binary mode
    fileStream.open(filename, std::ios::in | std::ios::out | std::ios::binary);

    // Check if file does not exist or is not open for some reason
    if (!fileStream.is_open()) {
        // File does not exist, so create it
        fileStream.open(filename, std::ios::out | std::ios::binary);
        if (!fileStream.is_open()) {
            throw std::runtime_error("Unable to create file: " + filename);
        }
        // File created, close the file
        fileStream.close();
        
        // Re-open the file in read/write mode
        fileStream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!fileStream.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }
    }

    // Determine the record count
    fileStream.seekg(0, std::ios::end);
    recordCount = fileStream.tellg() / sizeof(T);
    fileStream.seekg(0, std::ios::beg);
}


template<typename T>
void EntityIO<T>::exit() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

template<typename T>
void EntityIO<T>::seekToStart() {
    fileStream.clear();
    fileStream.seekg(0, std::ios::beg);
}

template<typename T>
void EntityIO<T>::seekTo(int offset) {
    fileStream.clear();
    fileStream.seekg(offset * sizeof(T), std::ios::beg);
}

template<typename T>
std::vector<T*> EntityIO<T>::readNRecords(int n) {
    std::vector<T*> records;
    for (int i = 0; i < n; ++i) {
        T* record = new T;
        if (fileStream.read(reinterpret_cast<char*>(record), sizeof(T))) {
            records.push_back(record);
        } else {
            delete record;
            records.push_back(nullptr);
        }
    }
    return records;
}

template<typename T>
T* EntityIO<T>::readRecord() {
    T* record = new T;
    if (fileStream.read(reinterpret_cast<char*>(record), sizeof(T))) {
        return record;
    }
    
    return nullptr;
}

template<typename T>
void EntityIO<T>::appendRecord(const T& record) {
    fileStream.clear();
    fileStream.seekp(0, std::ios::end);
    fileStream.write(reinterpret_cast<const char*>(&record), sizeof(T));
    ++recordCount;
}

template<typename T>
void EntityIO<T>::updateRecord(int index, const T& newRecord) {
    if (index < 0 || index >= recordCount) {
        throw std::out_of_range("Index out of range");
    }

    seekTo(index);
    fileStream.write(reinterpret_cast<const char*>(&newRecord), sizeof(T));
    if (!fileStream) {
        throw std::runtime_error("Failed to write to the file");
    }
}

template<typename T>
int EntityIO<T>::getRecordCount() {
    return recordCount;
}
