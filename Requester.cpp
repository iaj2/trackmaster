#include "Requester.h"
#include "FileNotOpenException.h"
#include "FileOpenFailedException.h"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

int Requester::recordCount = 10;

// ------------------------------------------------------------------------------------------------
// Opens the necessary files relevant for "Requester" records.
// ------------------------------------------------------------------------------------------------
void Requester::initRequester() {
    string filename = "Requesters.bin";
    requesterFile.open(filename, ios::in | ios::out | ios::binary);
    if (!requesterFile.is_open()) {
        throw FileOpenFailedException("File open failed");
    }
}
// ------------------------------------------------------------------------------------------------
// Seeks to start of the requester file.
// ------------------------------------------------------------------------------------------------
void Requester::startOfRequesterFile() {
    if (requesterFile.is_open()) {
        requesterFile.seekg(0, ios::beg);
    }
    else {
        throw FileNotOpenException("File not open");
    }
}

// ------------------------------------------------------------------------------------------------
// Move the change file pointer to an offset from the start
// -----------------------------------------------------------------------------------------------
void Requester::seekRequesterFile(int records_offset) {
    if (requesterFile.is_open()) {
        requesterFile.seekg(sizeof(Requester)*records_offset, ios::beg);
    } else {
        // if the file isnt open throw an exception 
        throw FileNotOpenException("Requester file is not open");
    }
}
// ------------------------------------------------------------------------------------------------
// Gets requester record currently pointed to in file.
// ------------------------------------------------------------------------------------------------
Requester* Requester::getRequesterRecord() {
    if (requesterFile.is_open()) {
        Requester* currentRequester = new Requester();
        requesterFile.read(reinterpret_cast<char*>(&currentRequester), sizeof(Requester));
        return currentRequester;
    }
    else {
        throw FileNotOpenException("File not open");
    }
    return nullptr; // if end of file
}

// Takes in the created "Requester" object to write to file.
void Requester::recordRequester(Requester newRequester) {
    if (requesterFile.is_open()) {
       requesterFile.write(reinterpret_cast<char*>(&newRequester), sizeof(Requester));
       // increase count
       recordCount++;
    }
    else {
        throw FileNotOpenException("File not open");
    }
}

/*
// Generates a report for all requesters.
void Requester::reportAllRequester() {
    ofstream report_file("Requester_Report.bin");
    startOfRequesterFile();
    char email[MAX_EMAIL_LENGTH + 1];
    char name[MAX_NAME_LENGTH + 1];
    int phone;
    char department[MAX_DEPARTMENT_LENGTH + 1];
    while (requester_file >> email >> name >> phone >> department) {
        report_file << email << " " << name << " " << phone << " " << department << endl;
    }
    report_file.close();
}
*/

// Exits the Requester Operation.
void Requester::exitRequester() {
    if (requesterFile.is_open()) {
        requesterFile.close();
    }
    else {
        throw FileNotOpenException("File not open");
    }
}

// Default Constructor to create a "Requester" Object.
Requester::Requester() {
    this->requesterEmail[0] = '\0';
    this->name[0] = '\0';
    this->phone = 0;
    this->department[0] = '\0';

}

// Parameterized Constructor to create a "Requester" Object. This requires the input of all private data attributes of the "Requester" class.
Requester::Requester(const char* requesterEmail, const char* name, int phone, const char* department) {
    strncpy(this->requesterEmail, requesterEmail, sizeof(this->requesterEmail) - 1); 
    this->requesterEmail[sizeof(this->requesterEmail) - 1] = '\0'; // Ensure null termination
    strncpy(this->name, name, sizeof(this->name) - 1);
    this->name[sizeof(this->name) - 1] = '\0'; // Ensure null termination
    strncpy(this->department, department, sizeof(this->department) - 1);
    this->department[sizeof(this->department) - 1] = '\0'; // Ensure null termination
    this->phone = phone;
}

// Function to retrieve the "Requester" object's requesterEmail.
const char* Requester::getRequesterEmail() const {
    return requesterEmail;
}

// Function to retrieve the "Requester" object's name.
const char* Requester::getName() const {
    return name;
}

// Function to retrieve the "Requester" object's phone number.
int Requester::getPhone() const {
    return phone;
}

// Function to retrieve the "Requester" object's department.
const char* Requester::getDepartment() const {
    return department;
}


// Function to retrieve the "Requester" object's record count.
int Requester::getRequesterCount() {
    return recordCount;
}

// Function to set the "Requester" object's requesterEmail.
void Requester::setRequesterEmail(const char* newRequesterEmail) {
    strncpy(requesterEmail, newRequesterEmail, sizeof(requesterEmail) - 1); 
    requesterEmail[sizeof(requesterEmail) - 1] = '\0'; // Ensure null termination
}

// Function to set the "Requester" object's name.
void Requester::setName(const char* newName) {
    strncpy(name, newName, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0'; // Ensure null termination
}

// Function to set the "Requester" object's phone number.
void Requester::setPhone(const int newPhone) {
    phone = newPhone;
}

// Function to set the "Requester" object's department.
void Requester::setDepartment(const char* newDepartment) {
    strncpy(department, newDepartment, sizeof(department) - 1);
    department[sizeof(department) - 1] = '\0'; // Ensure null termination
}