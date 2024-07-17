

/*

Request.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin and Isaac James

--------------------------------------------------------------------------------------------------------------------

This module contains the class for the "Request" entity. This contains the following private data attributes:

Primary Attributes:
1. changeID         (foreign key to "Change" entity)
2. requesterEmail   (foreign key to "Requester" entity)

Non-Primary Attributes:
3. productName      (foreign key to "ProductRelease" entity)
4. productReleaseID (foreign key to "ProductRelease" entity)
5. priority


This exported class contains public functions that allow the user to retrieve the attributes above, as well as set the priority of the request.

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef REQUEST_H
#define REQUEST_H

#include "Priority.h"

#include "FileNotOpenException.h"
#include "FileOpenFailedException.h"
#include "RecordNotFoundException.h"

class Request {

    public:

    // --------------------------------------------------------------------------------------------------------------------

        /* Opens the necessary files relevant for "Request" records.

        Parameters: None

        Return: Does not return anything.
        */
        static void initRequest();

    // --------------------------------------------------------------------------------------------------------------------

        /* Seeks to start of the request file.

        Parameters: None

        Return: Does not return anything.

        */
        static void startOfRequestFile();

    // --------------------------------------------------------------------------------------------------------------------

        /* Gets request record currently pointed to in file.

        Parameters: None

        Return: Reference to the request record or nullptr if end of file.

        */
        static Request* getRequestRecord();
    // --------------------------------------------------------------------------------------------------------------------

        /* Searches and returns the request record associated with the given changeID

        Parameters (changeID): In parameter.

        Return: Request object that has the associated changeID. Otherwise returns nullptr.

        */


        static Request* retrieveRequestRecord(const int changeID);

    // --------------------------------------------------------------------------------------------------------------------

        /* Takes in the created "Request" object to write to file.

        Parameters (Request newRequest): In parameter.

        Return: Does not return anything.
        */

        static void recordRequest(Request &newRequest);

    // --------------------------------------------------------------------------------------------------------------------

        /* Generates a report for a specific request.

        Parameters: None

        Return: Does not return anything.
        */
        void reportRequest();

    // --------------------------------------------------------------------------------------------------------------------

        /* Generates a report for all requests.

        Parameters: None

        Return: Does not return anything.
        */
        void reportAllRequests();

    // --------------------------------------------------------------------------------------------------------------------

        /* Exits the Request Operation.

        Parameters: None

        Return: Does not return anything.
        */
        static void exitRequest();

    //--------------------------------------------------------------------------------------------------------------------
            
        /* Default Constructor
    
        Parameters: None
    
        Return: Does not return anything.
        */
        Request();


    // --------------------------------------------------------------------------------------------------------------------

        /* Parameterized Constructor to create a Request Object. This requires the input of all private data attributes of the Request class.
    
        Parameter 1 (const int changeID): In parameter.
        Parameter 2 (const char* requesterEmail): In parameter.
        Parameter 3 (const char* productName): In parameter.
        Parameter 4 (const int productReleaseID): In parameter.
        Parameter 5 (Priority priority): In parameter.
    
        Return: Does not return anything.
        */
        Request(const int changeID, const char* requesterEmail, const char* productName, const int productReleaseID, Priority priority);
    
    // --------------------------------------------------------------------------------------------------------------------
        
        /* Copy Constructor to create a new Request Object.
    
        Parameter 1 (const Request& other): In parameter.
    
        Return: Does not return anything.
        */

        Request(const Request& other);

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Request" object's changeID.

        No parameters.

        Return: "Request" object's changeID as an integer.
        */
        int getChangeID();

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Request" object's requesterEmail.

        No parameters.

        Return: "Request" object's requesterEmail as an character array of maximum size 30.
        */
        char* getRequesterEmail();

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Request" object's productName.

        No parameters.

        Return: "Request" object's productName as an character array of maximum size 15.
        */
        char* getProductName();

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Request" object's productReleaseID.

        No parameters.

        Return: "Request" object's productReleaseID as an integer.
        */
        int getProductReleaseID();

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Request" object's priority.

        No parameters.

        Return: "Request" object's priority as an integer (1-5).
        */
        Priority getPriority();

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "Request" object's priority.

        Parameter 1 (Priority priority): In parameter.

        Return: Does not return anything
        */
        void setPriority(Priority newPriority);

    // --------------------------------------------------------------------------------------------------------------------


    private:

        static const int MAX_EMAIL_LENGTH = 30;
        static const int MAX_PRODUCT_NAME_LENGTH = 15;

        int changeID;
        char requesterEmail[MAX_EMAIL_LENGTH + 1];
        char productName[MAX_PRODUCT_NAME_LENGTH + 1];
        int productReleaseID;
        Priority priority;

        static int recordCount;
};

#endif
