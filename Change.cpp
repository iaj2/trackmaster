/*
Change.cpp
--------------------------------------------------------------------------------------------------------------------
Revision History:
Rev. 1 - 2024/07/13 Original by Bowen Jin
Rev. 2 - 2024/07/30 
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related to the header file 
--------------------------------------------------------------------------------------------------------------------
*/

#include "Change.h"
#include "FileOpenFailedException.h"
#include "FileNotOpenException.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

// ------------------------------------------------------------------------------------------------
// Default constructor
// ------------------------------------------------------------------------------------------------
Change::Change() : status(Status::Open), productName(""), anticipatedReleaseID(0),
    description(""), date(0)
  {}

// ------------------------------------------------------------------------------------------------
// Parameterized Constructor
// ------------------------------------------------------------------------------------------------
Change::Change(
  Status status, const char* productName, const int anticipatedReleaseID, 
  const char* description, const int date
  ) : changeID(changeID), status(status), anticipatedReleaseID(anticipatedReleaseID), date(date) 
  {
    // set auto-generated change id


    // Take product name up to its max length
    strncpy(this->productName, productName, MAX_PRODUCT_NAME_LENGTH);
    this->productName[MAX_PRODUCT_NAME_LENGTH] = '\0'; // set last character to null

    // Take description up to its max length
    strncpy(this->description, description, MAX_DESCRIPTION_LENGTH);
    this->description[MAX_DESCRIPTION_LENGTH] = '\0';
}

// ------------------------------------------------------------------------------------------------
// Get change ID
// -----------------------------------------------------------------------------------------------
int Change::getchangeID() const { return changeID; }

// ------------------------------------------------------------------------------------------------
// Get change status
// -----------------------------------------------------------------------------------------------
Change::Status Change::getStatus() const { return status; }

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

// ------------------------------------------------------------------------------------------------
// Set the change ID
// -----------------------------------------------------------------------------------------------
void Change::setchangeID() { 
    srand(static_cast<unsigned int>(time(nullptr)));
    int num = rand() % 9000 + 1000;
    this->changeID = num;
}

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

// ------------------------------------------------------------------------------------------------
// Convert status enum to string
// -----------------------------------------------------------------------------------------------
string Change::statusToString(Change::Status status) {
    switch (status) {
        case Change::Status::Assessed:
            return "Assessed";
        case Change::Status::Open:
            return "Open";
        case Change::Status::Canceled:
            return "Canceled";
        case Change::Status::In_Progress:
            return "In Progress";
        default:
            return "";
    }
}