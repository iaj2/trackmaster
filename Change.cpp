#include <iostream>
#include <fstream>
#include <cstring>
#include "Change.h"

using namespace std;

// Static member variables
std::fstream Change::changeFile;

// Default Constructor
Change::Change() : changeID(0), status(Status::Open), productName(""), anticipatedReleaseID(0),
    description(""), date(0)
  {}

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

void Change::initChange() {
    char changeFileName[] = "changes.dat";

    // Open file for input/output in binary mode
    changeFile.open(changeFileName, ios::out | ios::in | ios::binary );

    // If file failed to open
    if(!changeFile) {
        // create file if it does not exist
        changeFile.open(changeFileName, ios::out | ios::binary);

        // close & reopen
        changeFile.close();
        changeFile.open(changeFileName, ios::in | ios::out | ios::binary);
    }


}

void Change::startOfChangeFile() {
    changeFile.seekg(0, ios::beg);
}

Change* Change::getChangeRecord() {
    // return nullptr if at end of file
    if (changeFile.eof()) return nullptr;

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
}

void Change::recordChange(Change newChange) {
    // seek to end of file
    changeFile.seekp(0, ios::end);

    // write newChnage information to a record in file
    changeFile.write(reinterpret_cast<const char*>(&newChange), sizeof(Change));
}

void Change::exitChange() {
    if (changeFile.is_open()) {
        changeFile.close();
    }
}

// Private functions to retrieve the attributes
int Change::getchangeID() const { return changeID; }

Status Change::getStatus() const { return status; }

const char* Change::getProductName() const { 
    return productName; 
}

int Change::getAnticipatedReleaseID() const { return anticipatedReleaseID; }

const char* Change::getDescription() const { 
    return description;
}

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

void Change::setchangeID(const int newChangeID) { changeID = newChangeID; }

void Change::setStatus(Status newStatus) { status = newStatus; }

void Change::setAnticipatedReleaseID(const int newAnticipatedReleaseID) { 
    anticipatedReleaseID = newAnticipatedReleaseID; 
}

void Change::setDescription(const char* newDescription) {
    strncpy(description, newDescription, MAX_DESCRIPTION_LENGTH);
    description[MAX_DESCRIPTION_LENGTH] = '\0';
}
void Change::setDate(const int newDate) { date = newDate; }
