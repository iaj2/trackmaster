#include "Requester.h"
#include "FileNotOpenException.h"
#include "FileOpenFailedException.h"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

// Opens the necessary files relevant for "Requester" records.
void Requester::initRequester() {
    string filename = "Requesters.bin";
    requester_file.open(filename, ios::binary);
    if (!requester_file.is_open()) {
        throw FileOpenFailedException("File open failed");
    }
}

// Seeks to start of the requester file.
void Requester::startOfRequesterFile() {
    if (requester_file.is_open()) {
        requester_file.seekg(0, ios::beg);
    }
    else {
        throw FileNotOpenException("File not open");
    }
}

// Gets requester record currently pointed to in file.
Requester* Requester::getRequesterRecord() {
    char email[MAX_EMAIL_LENGTH + 1];
    char name[MAX_NAME_LENGTH + 1];
    int phone;
    char department[MAX_DEPARTMENT_LENGTH + 1];
    if (requester_file >> email >> name >> phone >> department) {
        return new Requester(email, name, phone, department);
    }
    return nullptr;
}

// Takes in the created "Requester" object to write to file.
void Requester::recordRequester(Requester newRequester) {
    if (requester_file.is_open()) {
        requester_file << newRequester.getRequesterEmail() << " " << newRequester.getName() << " " << newRequester.getPhone() << " " << newRequester.getDepartment() << endl;
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
    if (requester_file.is_open()) {
        requester_file.close();
    }
    else {
        throw FileNotOpenException("File not open");
    }
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