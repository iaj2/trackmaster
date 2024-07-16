#include <iostream>
#include <fstream>
#include <cstring>
#include "Change.h"
#include "FileOpenFailedException.h"
#include "FileNotOpenException.h"

using namespace std;

// ------------------------------------------------------------------------------------------------
// Default constructor
// ------------------------------------------------------------------------------------------------
Change::Change() : changeID(0), status(Status::Open), productName(""), anticipatedReleaseID(0),
    description(""), date(0)
  {}

// ------------------------------------------------------------------------------------------------
// Parameterized Constructor
// ------------------------------------------------------------------------------------------------
Change::Change(
  const int changeID, Status status, const char* productName, const int anticipatedReleaseID, 
  const char* description, const int date
  ) : changeID(changeID), status(status), anticipatedReleaseID(anticipatedReleaseID), date(date) 
  {
    // Take product name up to its max length
    strncpy(this->productName, productName, MAX_PRODUCT_NAME_LENGTH);
    this->productName[MAX_PRODUCT_NAME_LENGTH] = '\0'; // set last character to null

    // Take description up to its max length
    strncpy(this->description, description, MAX_DESCRIPTION_LENGTH);
    this->description[MAX_DESCRIPTION_LENGTH] = '\0';
}

// ------------------------------------------------------------------------------------------------
// Initialize the change file
// Opens the change file or creates it if it doesn't exist.
// ------------------------------------------------------------------------------------------------
void Change::initChange() {
    char changeFileName[] = "changes.dat";

    // Open file for input/output in binary mode
    changeFile.open(changeFileName, ios::out | ios::in | ios::binary );

     if (!changeFile) {
        // If the file does not exist, create it
        std::fstream { changeFileName};
        // Open for reading and writing
        changeFile.open(changeFileName, std::ios::binary);
    }
    // if open does not work then throw an exception 
    if (!changeFile) {
       throw FileOpenFailedException("Change file open failed");
    }
}

// ------------------------------------------------------------------------------------------------
// Move the change file pointer to the start
// -----------------------------------------------------------------------------------------------
void Change::startOfChangeFile() {
    if (changeFile.is_open()) {
        changeFile.seekp(0, std::ios::beg);
    } else {
        // if the file isnt open throw an exception 
        throw FileNotOpenException("Change file is not open");
    }
}

// ------------------------------------------------------------------------------------------------
// Move the change file pointer to an offset from the start
// -----------------------------------------------------------------------------------------------
void Change::seekChangeFile(int records_offset) {
    if (changeFile.is_open()) {
        changeFile.seekg(sizeof(Change)*records_offset, ios::beg);
    } else {
        // if the file isnt open throw an exception 
        throw FileNotOpenException("Change file is not open");
    }
}

// ------------------------------------------------------------------------------------------------
// Method to get a change record from the file
// Reads a change record from the currrent file pointer position.
// -----------------------------------------------------------------------------------------------
Change* Change::getChangeRecord() {
    // check if the file is open 
    if (changeFile.is_open()) {

        // return nullptr if at end of file
        if (changeFile.eof()) {
            return nullptr;
        }

        // create blank change object   
        Change* change = new Change();

        // read change record into object
        changeFile.read(reinterpret_cast<char*>(change), sizeof(change));

        // if record not fully read, return nullptr
        if (changeFile.fail() || changeFile.eof()) {
            delete change;
            return nullptr;
        }
        return change; 
    // throw an exception if the file is not found 
    } else {
        throw FileNotOpenException("Change file is not open");
    }
}

// ------------------------------------------------------------------------------------------------
// Record a new change to the end of the file
// -----------------------------------------------------------------------------------------------
const void Change::recordChange(Change newChange) {
    // make sure the file is open 
    if (changeFile.is_open()) {
        // seek to the end 
        changeFile.seekp(0, std::ios::end);

        // write it to file 
        changeFile.write(reinterpret_cast<char*>(&newChange), sizeof(Change));
    // throw an exception if the file is not open 
    } else {
        throw FileNotOpenException("File is not open");
    }
}

// ------------------------------------------------------------------------------------------------
// Close the change file
// -----------------------------------------------------------------------------------------------
void Change::exitChange() {
    if (changeFile.is_open()) {
        changeFile.close();
    }
}

// ------------------------------------------------------------------------------------------------
// Get change ID
// -----------------------------------------------------------------------------------------------
int Change::getchangeID() const { return changeID; }

// ------------------------------------------------------------------------------------------------
// Get change status
// -----------------------------------------------------------------------------------------------
Status Change::getStatus() const { return status; }

// ------------------------------------------------------------------------------------------------
// Get product name of the change
// -----------------------------------------------------------------------------------------------
const char* Change::getProductName() const { 
    return productName; 
}

// ------------------------------------------------------------------------------------------------
// Move the Anticipated release ID of the change
// -----------------------------------------------------------------------------------------------
int Change::getAnticipatedReleaseID() const { return anticipatedReleaseID; }

// ------------------------------------------------------------------------------------------------
// Get the description of the change
// -----------------------------------------------------------------------------------------------
const char* Change::getDescription() const { 
    return description;
}

// ------------------------------------------------------------------------------------------------
// Get the date of the change
// -----------------------------------------------------------------------------------------------
char* Change::getDate() const { 
    // Buffer to hold the formatted date (YYYY/MM/DD + null terminator)
    char *formattedDate = new char[11]; // YYYY/MM/DD + null terminator

    // Extract year, month, and day from the integer date
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;

    // Format the date into the char array
    std::sprintf(formattedDate, "%04d/%02d/%02d", year, month, day);

    // Return a pointer to the formatted date string
    return formattedDate;
}

// Private functions to set the attributes

// ------------------------------------------------------------------------------------------------
// Set the change ID
// -----------------------------------------------------------------------------------------------
void Change::setchangeID(const int newChangeID) { changeID = newChangeID; }

// ------------------------------------------------------------------------------------------------
// Set the status
// -----------------------------------------------------------------------------------------------
void Change::setStatus(Status newStatus) { status = newStatus; }

// ------------------------------------------------------------------------------------------------
// Set the anticipated release ID
// -----------------------------------------------------------------------------------------------
void Change::setAnticipatedReleaseID(const int newAnticipatedReleaseID) { 
    anticipatedReleaseID = newAnticipatedReleaseID; 
}

// ------------------------------------------------------------------------------------------------
// Set the description
// -----------------------------------------------------------------------------------------------
void Change::setDescription(const char* newDescription) {
    strncpy(description, newDescription, MAX_DESCRIPTION_LENGTH);
    description[MAX_DESCRIPTION_LENGTH] = '\0';
}

// ------------------------------------------------------------------------------------------------
// Set the date
// -----------------------------------------------------------------------------------------------
void Change::setDate(const int newDate) { date = newDate; }
