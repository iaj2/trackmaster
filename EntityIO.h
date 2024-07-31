/* 
EntityIO.h
--------------------------------------------------------------------------------------------------------------------
Rev. 1 - 2024/07/23 Original by Isaac James
--------------------------------------------------------------------------------------------------------------------
This class template provides an interface for input/output operations on entities of type T.
It supports operations such as initialization, reading, writing, and updating records in a file.
--------------------------------------------------------------------------------------------------------------------
Template Parameters:
- T: The type of the entities to be handled by this class.
*/

#ifndef ENTITYIO_H
#define ENTITYIO_H

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

template<typename T>
class EntityIO {
public:
// --------------------------------------------------------------------------------------------------------------------
    /* 
    Constructor

    Initializes the EntityIO object with the specified filename.

    Parameters:
    - filename: A string representing the name of the file to operate on.
    */
    EntityIO(const std::string& filename);
// --------------------------------------------------------------------------------------------------------------------

    /* 
    Initializes the file for I/O operations.
    Opens the file stream and sets up the necessary resources.
    */
    void init();
// --------------------------------------------------------------------------------------------------------------------

    /* 
    Exits the I/O operations.
    Closes the file stream and releases any resources.
    */
    void exit();
// --------------------------------------------------------------------------------------------------------------------
    
    /* 
    Seeks the file pointer to the start of the file.
    */
    void seekToStart();
// --------------------------------------------------------------------------------------------------------------------

    /* 
    Seeks the file pointer to a specific offset.

    Parameters:
    - offset: An integer representing the offset to seek to.
    */
    void seekTo(int offset);
// --------------------------------------------------------------------------------------------------------------------

    /* 
    Reads a specified number of records from the file.

    Parameters:
    - n: An integer representing the number of records to read.

    Returns:
    - A vector of pointers to the read records of type T.
    */
    std::vector<T*> readNRecords(int n);
// --------------------------------------------------------------------------------------------------------------------

    /* 
    Reads a single record from the file.

    Returns:
    - A pointer to the read record of type T.
    */
    T* readRecord();
// --------------------------------------------------------------------------------------------------------------------

    /* 
    Appends a new record to the file.

    Parameters:
    - record: A constant reference to the record of type T to append.
    */
    void appendRecord(const T& record);
// --------------------------------------------------------------------------------------------------------------------

    /* 
    Updates an existing record in the file.

    Parameters:
    - index: An integer representing the index of the record to update.
    - newRecord: A constant reference to the new record of type T to replace the old one.
    */
    void updateRecord(int index, const T& newRecord);
// --------------------------------------------------------------------------------------------------------------------

    /* 
    Gets the count of records in the file.

    Returns:
    - An integer representing the number of records in the file.
    */
    int getRecordCount();
// --------------------------------------------------------------------------------------------------------------------

private:
    std::fstream fileStream;
    std::string filename;
    int recordCount;

};

// Include the implementation of the template class in the header file
#include "EntityIO.tpp"

#endif // ENTITYIO_H
