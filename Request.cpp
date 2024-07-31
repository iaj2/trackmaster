/*
Request.cpp
--------------------------------------------------------------------------------------------------------------------
Revision History:
Rev. 1 - 2024/07/13 Original by Bowen Jin
Rev. 2 - 2024/07/30 
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related to the header file 
--------------------------------------------------------------------------------------------------------------------
*/
#include "Request.h"
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

//   ----------------------------------------------------------------
/*

Function: Default Constructor

*/

Request::Request(): changeID(0), requesterEmail(""), productName(""), productReleaseID(0), priority(Request::Priority::LOW) {}


//   ----------------------------------------------------------------
/*

Function: Parameterized Constructor

*/

Request::Request(const int changeID, const char* requesterEmail, const char* productName, const int productReleaseID, Priority priority) {

    this->changeID = changeID;
    this->productReleaseID = productReleaseID;
    this->priority = priority;

    strncpy(this->requesterEmail, requesterEmail, MAX_EMAIL_LENGTH);
    this->requesterEmail[MAX_EMAIL_LENGTH] = '\0';

    strncpy(this->productName, productName, MAX_PRODUCT_NAME_LENGTH);
    this->productName[MAX_PRODUCT_NAME_LENGTH] = '\0';
}

//   ----------------------------------------------------------------
/*

Function: Copy Constructor

This will be used for searchRequestRecord().

*/

Request::Request(const Request& other) {
    this->changeID = other.changeID;
    strcpy(this->requesterEmail, other.requesterEmail);
    strcpy(this->productName, other.productName);
    this->productReleaseID = other.productReleaseID;
    this->priority = other.priority;
}

//   ----------------------------------------------------------------

int Request::getChangeID() const {
    return changeID;
}

//   ----------------------------------------------------------------

const char* Request::getRequesterEmail() const {
    return requesterEmail;
}

//   ----------------------------------------------------------------

const char* Request::getProductName() const {
    return productName;
}

//   ----------------------------------------------------------------

int Request::getProductReleaseID() const {
    return productReleaseID;
}

//   ----------------------------------------------------------------

Request::Priority Request::getPriority() const {
    return priority;
}

//   ----------------------------------------------------------------

void Request::setPriority(Priority newPriority) {
    priority = newPriority;
}
