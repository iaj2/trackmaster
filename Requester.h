

/*

Requester.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin and Isaac James
Rev. 2 - 2024/07/14 by Kwan Wai Sit

--------------------------------------------------------------------------------------------------------------------

This module contains the class for the "Requester" entity. This contains the following private data attributes:

Primary Attributes:
1. requesterEmail            (foreign key to "Request" entity)

Non-Primary Attributes:
2. name
3. phone


This exported class contains public functions that allow the user to retrieve and set the attributes above.

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef REQUESTER_H
#define REQUESTER_H
#include <fstream>

class Requester {

    public:

    // --------------------------------------------------------------------------------------------------------------------

        /* Opens the necessary files relevant for "Requester" records.

        Parameters: None

        Return: Does not return anything.
        */
        static void initRequester();

    // --------------------------------------------------------------------------------------------------------------------

        /* Seeks to start of the requester file.

        Parameters: None

        Return: Does not return anything.

        */
        static void startOfRequesterFile();

    // --------------------------------------------------------------------------------------------------------------------

        /* Seeks to an offset from the start of the requester file.

        Parameters: None

        Return: Does not return anything.
        
        */
        static void seekRequesterFile(int records_offset);

    // --------------------------------------------------------------------------------------------------------------------

        /* Gets requester record currently pointed to in file.

        Parameters: None

        Return: reference to the requester record or nullptr if end of file.

        */
        static Requester* getRequesterRecord();
    // --------------------------------------------------------------------------------------------------------------------

        /* Takes in the created "Requester" object to write to file.
        
        Parameters (Requester newRequester): In parameter.
        
        Return: Does not return anything.
        */
        
        static void recordRequester(Requester newRequester);

    // --------------------------------------------------------------------------------------------------------------------

        /* Generates a report for all requesters.

        Parameters: None

        Return: Does not return anything.
        */
        // void reportAllRequester();

    // --------------------------------------------------------------------------------------------------------------------

        /* Exits the Requester Operation.

        Parameters: None

        Return: Does not return anything.
        */
        static void exitRequester();

    // --------------------------------------------------------------------------------------------------------------------

        /* Default Constructor to create a "Requester" Object.

        Parameters: None

        Return: Does not return anything.
        */
        Requester();

    // --------------------------------------------------------------------------------------------------------------------

        /* Parameterized Constructor to create a "Requester" Object. This requires the input of all private data attributes of the "Requester" class.
    
        Parameter 1 (const char* requesterEmail): In parameter.
        Parameter 2 (const char* name): In parameter.
        Parameter 3 (int phone): In parameter.
        Parameter 4 (department): In parameter.
    
        Return: Does not return anything.
        */
        Requester(const char* requesterEmail, const char* name, int phone, const char* department);

    // --------------------------------------------------------------------------------------------------------------------
        
        /* Function to retrieve the "Requester" object's requesterEmail.

        No parameters.

        Return: "Requester" object's requesterEmail as an character array of maximum size 30.
        */
        const char* getRequesterEmail() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Requester" object's name.

        No parameters.

        Return: "Requester" object's name as an character array of maximum size 15.
        */
        const char* getName() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Requester" object's phone number.

        No parameters.

        Return: "Requester" object's phone number as a 10-digit integer.
        */
        int getPhone() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Requester" object's department.

        No parameters.

        Return: "Requester" object's department as an character array of maximum size 15.
        */
        const char* getDepartment() const;
    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Requester" object's record count.

        No parameters.

        Return: "Requester" object's record count.
        */
        static int getRequesterCount();

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "Requester" object's requesterEmail.

        Parameter 1 (const char* newRequesterEmail): In parameter.

        Return: Does not return anything.
        */
        void setRequesterEmail(const char* newRequesterEmail);

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "Requester" object's name.

        Parameter 1 (const char* newName): In parameter.

        Return: Does not return anything
        */
        void setName(const char* newName);

    // --------------------------------------------------------------------------------------------------------------------
        /* Function to set the "Requester" object's phone number.

        Parameter 1 (const int newPhone): In parameter.

        Return: Does not return anything
        */
        void setPhone(const int newPhone);
    
    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "Requester" object's department.

        Parameter 1 (const char* newName): In parameter.

        Return: Does not return anything
        */
        void setDepartment(const char* newDepartment);

    // --------------------------------------------------------------------------------------------------------------------

    private:

        static const int MAX_EMAIL_LENGTH = 30;
        static const int MAX_NAME_LENGTH = 15;
        static const int MAX_DEPARTMENT_LENGTH = 10;
        static int requesterCount;

        char requesterEmail[MAX_EMAIL_LENGTH + 1];
        char name[MAX_NAME_LENGTH + 1];
        int phone;
        char department[MAX_DEPARTMENT_LENGTH + 1];

        static std::fstream requesterFile;
};

#endif
