/*

Change.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin and Isaac James

--------------------------------------------------------------------------------------------------------------------

This module contains the class for the "Change" entity. This contains the following private data attributes:

Primary Attributes:
1. changeID             (foreign key to "Request" entity)

Non-Primary Attributes:
2. status
3. productName          (foreign key to "Product" and "ProductRelease" entity)
4. anticipatedReleaseID (foreign key to "ProductRelease" entity)
5. description
6. date


This exported class contains public functions that allow the user to retrieve the attributes and set all except for productName.

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef CHANGE_H
#define CHANGE_H
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Change {

    public:
        enum class Status {Open, Assessed, In_Progress, Done, Canceled};

    // --------------------------------------------------------------------------------------------------------------------
        /* Default constructor to create an unanitialized "Change" object
        */
        Change();

    // --------------------------------------------------------------------------------------------------------------------
        /* Parameterized Constructor to create a "Change" Object.
        This requires the input of all private data attributes of the "Change" class.

        Parameter 1 (const int changeID): In parameter.
        Parameter 2 (Status status): In parameter.
        Parameter 3 (const char* productName): In parameter.
        Parameter 4 (const int anticipatedReleaseID): In parameter.
        Parameter 5 (const char* description): In parameter.
        Parameter 6 (const int date): In parameter.

        Return: Does not return anything.
        */
        Change(const int changeID, Status status, const char* productName, const int anticipatedReleaseID, const char* description, const int date);

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Change" object's changeID.

        No parameters.

        Return: "Change" object's changeID as a unique combination of integers.
        */
        int getchangeID() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Change" object's status.

        No parameters.

        Return: "Change" object's status variable which can be only one of Open, Assessed, In_Progress, Done, or Canceled
        */
        Status getStatus() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Change" object's productName.

        No parameters.

        Return: "Change" object's productName as an character array of maximum size 15.
        */
        const char* getProductName() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Change" object's anticipatedReleaseID.

        No parameters.

        Return: "Change" object's anticipatedReleaseID as a unique combination of integers.
        */
        int getAnticipatedReleaseID() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Change" object's description.

        No parameters.

        Return: "Change" object's description as an character array of maximum size 30.
        */
        const char* getDescription() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Change" object's date.

        No parameters.

        Return: "Change" object's date as an 8-digit integer formatted in YYYY/MM/DD
        */
        char* getDate() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "Change" object's changeID.

        Parameter 1 (const char* newChangeID): In parameter (must be unique combination of integers).

        Return: Does not return anything.
        */
        void setchangeID(const int newChangeID);

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Change" object's status.

        Parameter 1 (Status status): In parameter (must be 1 of Open, Assessed, In_Progress, Done, or Canceled)

        Return: Does not return anything.
        */
        void setStatus(Status status);

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "Change" object's anticipatedReleaseID.

        Parameter 1 (const int newAnticipatedReleaseID): In parameter

        Return: Does not return anything.
        */
        void setAnticipatedReleaseID(const int newAnticipatedReleaseID);

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "Change" object's description.

        Parameter 1 (const char* newDescription): In parameter (limited to 30 characters).

        Return: Does not return anything.
        */
        void setDescription(const char* newDescription);

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Change" object's date.

        Parameter 1 (const int newDate): In parameter (must be exactly 8 digits formatted as YYYY/MM/DD).

        Return: Does not return anything.
        */
        void setDate(const int newDate);
    // --------------------------------------------------------------------------------------------------------------------
        /* Function to get status as a string 
        
        Paramater 1 (Status status): In parameter

        Return: status formatted as a string
        */
        static string statusToString(Status status);

        static const int MAX_PRODUCT_NAME_LENGTH = 15;
        static const int MAX_DESCRIPTION_LENGTH = 30;

    private:
        int changeID;
        Status status;
        char productName[MAX_PRODUCT_NAME_LENGTH + 1];
        int anticipatedReleaseID;
        char description[MAX_DESCRIPTION_LENGTH + 1];
        int date;
};

#endif
