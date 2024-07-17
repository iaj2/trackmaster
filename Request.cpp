
#include <iostream>
#include <fstream>

#include "Request.h"

using namespace std;


fstream Request::requestFile;


// --------------------------------------------------------------------------------------------------------------------
/*

Function: initRequest()
This function initializes the request operation by attempting to open an existing  binary file for read and write operations.
If the file does not exist, it creates a new binary file and then reopens it for read and write.
If the file cannot be opened, an exception is thrown.

*/

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

// --------------------------------------------------------------------------------------------------------------------
/*

Function: startOfRequestFile()

This function checks if the requestFile stream is open.
If the file is open, it sets the position of the next character to be extracted from the input stream to the 
beginning of the file.
If the file is not open, it throws a FileNotOpenException with an appropriate error message

*/

void Request::startOfRequestFile() {

    // Case 1: If open, seek to beginning
    if (requestFile.is_open()) {
        requestFile.seekg(0, ios::beg);
    }

    // Case 2: Cannot be opened
    else {
        throw FileNotOpenException("File is not open");
    }
}

// --------------------------------------------------------------------------------------------------------------------
/*

Function: getRequestRecord()

This function attempts to read a Request record from an open file. Before reading the record, it will check...

1. If the file is open
2. Is file pointer at EOF

If conditions are passed, function allocates memory for a new Request object and reads data from the file to this object
If the read operation is incomplete or the file pointer reaches EOF during reading, it deletes the allocated memory and returns
nullptr. Throws exception if the appropriate file is not open.

*/

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


// --------------------------------------------------------------------------------------------------------------------
/*

Function: searchRequestRecord()

Implements the search for a request record by changeID.
If the request file is open, it loops through the file to find a matching changeID.
When found, it creates a new Request object and returns it

- If the changeID is not found, it throws a RecordNotFoundException.
- If the file cannot be opened, it throws a FileNotOpenException.

*/


Request* Request::searchRequestRecord(const int changeID) {

    // Case 1: Request File is opened
    if (requestFile.is_open()) {
        Request* currentRequest;
        startOfRequestFile();

        // Loop through the file to find the matching changeID. Reads data equal to the size of Request object.
            // Each read checks if changeID in current block matches specified changeID provided
        while (requestFile.read(reinterpret_cast<char*>(&currentRequest), sizeof(Request))){

            // If found, create new Request object
            if (currentRequest->changeID == changeID) {
                Request* newRequest = Request(&currentRequest);
                return newRequest;
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

// --------------------------------------------------------------------------------------------------------------------
/*

Function: recordRequest()

This function records a new request into the request file by seeking past the last record and updating the recordCount.
Throws an exception if the file is not open.

*/

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


// --------------------------------------------------------------------------------------------------------------------
/*

Function: exitRequest()

*/

void Request::exitRequest() {
    if (requestFile.is_open()) {
        requestFile.close();
    }
}

// --------------------------------------------------------------------------------------------------------------------
/*

Function: Default Constructor

*/

Request::Request() {}


// --------------------------------------------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------------------------------------------

int Request::getChangeID() {
    return changeID;
}

// --------------------------------------------------------------------------------------------------------------------

char* Request::getRequesterEmail() {
    return requesterEmail;
}

// --------------------------------------------------------------------------------------------------------------------

char* Request::getProductName() {
    return productName;
}

// --------------------------------------------------------------------------------------------------------------------

int Request::getProductReleaseID() {
    return productReleaseID;
}

// --------------------------------------------------------------------------------------------------------------------

Priority Request::getPriority() {
    return priority;
}

// --------------------------------------------------------------------------------------------------------------------

void Request::setPriority(Priority newPriority) {
    priority = newPriority;
}