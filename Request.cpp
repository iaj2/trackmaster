
#include <iostream>
#include <fstream>

#include "Request.h"

using namespace std;


static fstream requestFile;

// Open "request.bin" file. If file does not exist, create another. Otherwise, throw FileOpenFailedException
void Request::initRequest() {

    // Case 1: Binary file already exists. Open for read/write in binary
    requestFile.open("request.bin", ios::in | ios:: out | ios::binary);

    
    // Case 2: File does not exist. Create new file
    if (!requestFile.is_open()) {

        // ios::out will create a new file if non-existent
        requestFile.open("request.bin", ios::out | ios::binary);
        requestFile.close();

        // Re-open
        requestFile.open("request.bin", ios::in | ios:: out | ios::binary);
    }

    // Case 3: File cannot be opened
    if (!requestFile.is_open()) {
       throw FileOpenFailedException("File open failed");
    }
}


// Seek to beginning of file
void Request::startOfRequestFile() {
    if (requestFile.is_open()) {
        requestFile.seekg(0, ios::beg);
    }
    else {
        throw FileNotOpenException("File is not open");
    }
}


// Static function to get the change record currently pointed to in the file
Request* Request::getRequestRecord() {

    // Ensure requestFile is opened
    if (requestFile.is_open()) {

        // Error 1: Currently pointed to EOF
        if (requestFile.eof()) {
            return nullptr;
        }

        // Create new blank request object to fill
        Request* request = new Request();
        requestFile.read(reinterpret_cast<char*>(&request), sizeof(Request));

        // If record is not fully read, delete allocated "request" and return nullptr
        if (!request || requestFile.eof()) {
            delete request;
            return nullptr;
        }

        return request;
    }

    else {
        throw FileNotOpenException("File is not open");
    }
}

        /* Searches and returns the request record associated with the given changeID

        Parameters (changeID): In parameter.

        Return: Request object that has the associated changeID.

        */
Request* Request::retrieveRequestRecord(const int changeID) {

    // Case 1: Request File is opened
    if (requestFile.is_open()) {
        Request* currentRequest;
        startOfRequestFile();

        // Loop through the file to find the matching changeID.
        while (requestFile.read(reinterpret_cast<char*>(&currentRequest), sizeof(Request))){

            // If found, create new Request object
            if (currentRequest->changeID == changeID) {
                return currentRequest;
            }  
        }

        // Record does not exist
        throw RecordNotFoundException("Record not found");
    }
    
    // Case 2: File cannot be opened
    else {
        throw FileNotOpenException("File is not open");
    }
}

// Record a new request to the file
void Request::recordRequest(Request &newRequest) {

    if (requestFile.is_open()) {

        // Move past last written "Request" record
        requestFile.seekp((recordCount+1)*sizeof(Request));

        // Write the new request after the last record or at the end of the file
        requestFile.write(reinterpret_cast<char*>(&newRequest), sizeof(Request));
        requestFile.flush();

        recordCount++;
    }

    else {
        throw FileNotOpenException("File is not open");
    }
}


// Generate a report for all requests
void Request::reportAllRequests() {
    startOfRequestFile();
    Request* current = nullptr;
    while ((current = getRequestRecord()) != nullptr) {
        current->reportRequest();
        delete current;
    }
}

// Exit the request operation by closing the file
void Request::exitRequest() {
    if (requestFile.is_open()) {
        requestFile.close();
    }
}


// Default Constructor
Request::Request() {}

// Parameterized Constructor for all parameters
Request::Request(const int changeID, const char* requesterEmail, const char* productName, const int productReleaseID, Priority priority) {

    this->changeID = changeID;
    this->productReleaseID = productReleaseID;
    this->priority = priority;

    strncpy(this->requesterEmail, requesterEmail, MAX_EMAIL_LENGTH);
    this->requesterEmail[MAX_EMAIL_LENGTH] = '\0';

    strncpy(this->productName, productName, MAX_PRODUCT_NAME_LENGTH);
    this->productName[MAX_PRODUCT_NAME_LENGTH] = '\0';
}



// Copy Constructor for only changeID parameter. This will be used for retrieveRequestRecord()
Request::Request(const Request& other) {
    this->changeID = other.changeID;
    strcpy(this->requesterEmail, other.requesterEmail);
    strcpy(this->productName, other.productName);
    this->productReleaseID = other.productReleaseID;
    this->priority = other.priority;
}


int Request::getChangeID() {
    return changeID;
}


char* Request::getRequesterEmail() {
    return requesterEmail;
}


char* Request::getProductName() {
    return productName;
}


int Request::getProductReleaseID() {
    return productReleaseID;
}


Priority Request::getPriority() {
    return priority;
}


void Request::setPriority(Priority newPriority) {
    priority = newPriority;
}