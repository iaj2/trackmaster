/*
Entity.tpp
--------------------------------------------------------------------------------------------------------------------
Revision History:
Rev. 2 - 2024/07/23 Original by Isaac James 
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related to the header file 
--------------------------------------------------------------------------------------------------------------------
*/

#include "EntityIO.h"
#include <iostream>

using namespace std;

template<typename T>
EntityIO<T>::EntityIO(const string& filename) : filename(filename), recordCount(0) {}

template<typename T>
void EntityIO<T>::init() {
    // Attempt to open the file in read/write binary mode
    fileStream.open(filename, ios::in | ios::out | ios::binary);

    // Check if file does not exist or is not open for some reason
    if (!fileStream.is_open()) {
        // File does not exist, so create it
        fileStream.open(filename, ios::out | ios::binary);
        if (!fileStream.is_open()) {
            throw runtime_error("Unable to create file: " + filename);
        }
        // File created, close the file
        fileStream.close();
        
        // Re-open the file in read/write mode
        fileStream.open(filename, ios::in | ios::out | ios::binary);
        if (!fileStream.is_open()) {
            throw runtime_error("Unable to open file: " + filename);
        }
    }

    // Determine the record count
    fileStream.seekg(0, ios::end);
    recordCount = fileStream.tellg() / sizeof(T);
    fileStream.seekg(0, ios::beg);
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
    fileStream.seekg(0, ios::beg);
}

template<typename T>
void EntityIO<T>::seekTo(int offset) {
    fileStream.clear();
    fileStream.seekg(offset * sizeof(T), ios::beg);
}

template<typename T>
vector<T*> EntityIO<T>::readNRecords(int n) {
    vector<T*> records;
    for (int i = 0; i < n; ++i) {
        T* record = new T;
        if (fileStream.read(reinterpret_cast<char*>(record), sizeof(T))) {
            records.push_back(record);
        } else {
            break;
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
    fileStream.seekp(0, ios::end);
    fileStream.write(reinterpret_cast<const char*>(&record), sizeof(T));
    ++recordCount;
}

template<typename T>
void EntityIO<T>::updateRecord(int index, const T& newRecord) {
    if (index < 0 || index >= recordCount) {
        throw out_of_range("Index out of range");
    }

    seekTo(index);
    fileStream.write(reinterpret_cast<const char*>(&newRecord), sizeof(T));
    if (!fileStream) {
        throw runtime_error("Failed to write to the file");
    }
}

template<typename T>
int EntityIO<T>::getRecordCount() {
    return recordCount;
}
