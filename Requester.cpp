#include "Requester.h"
#include "FileNotOpenException.h"
#include "FileOpenFailedException.h"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

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