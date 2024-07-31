/*
InternalRequester.h
--------------------------------------------------------------------------------------------------------------------
Revision History:
Rev. 1 - 2024/07/02 Original by Bowen Jin and Isaac James
Rev. 2 - 2024/07/30 
--------------------------------------------------------------------------------------------------------------------
This module contains the class for the "InternalRequester" entity, which is a subclass of the "Requester" class. This contains the additional private data attribute "department".
This exported class contains public functions that allow the user to retrieve and set the additional attribute.
--------------------------------------------------------------------------------------------------------------------
*/

#ifndef INTERNALREQUESTER_H
#define INTERNALREQUESTER_H

#include "Requester.h"

class InternalRequester : public Requester {

    public:

    // --------------------------------------------------------------------------------------------------------------------

        /* Takes in the created "InternalRequester" object to write to file.
        
        Parameters: InternalRequester newInternalRequester. In parameter.
        
        Return: Does not return anything.
        */
        static void recordInternalRequester(InternalRequester newInternalRequester);

    // --------------------------------------------------------------------------------------------------------------------

        /* Writes the created "InternalRequester" object to file.

        Parameters: None

        Return: Does not return anything.
        */
        void reportAllInternalRequester();

    // --------------------------------------------------------------------------------------------------------------------

        /* Exits the InternalRequester Operation.

        Parameters: None

        Return: Does not return anything.
        */
        static void exitInternalRequester();

    // --------------------------------------------------------------------------------------------------------------------

        /* Parameterized Constructor to create an "InternalRequester" Object. This requires the input of all private data attributes of the "InternalRequester" class.

        Parameter 1 (const char* requesterEmail): In parameter.
        Parameter 2 (const char* name): In parameter.
        Parameter 3 (int phone): In parameter.
        Parameter 4 (const char* department): In parameter.

        Return: Does not return anything.
        */
        InternalRequester(const char* requesterEmail, const char* name, int phone, const char* department);

    // --------------------------------------------------------------------------------------------------------------------

private:

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "InternalRequester" object's department.

        No parameters.

        Return: "InternalRequester" object's department as a character array of maximum size 12.
        */
        char* getDepartment() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "InternalRequester" object's department.

        Parameter 1 (const char* newDepartment): In parameter.

        Return: Does not return anything.
        */
        void setDepartment(const char* newDepartment);

    // --------------------------------------------------------------------------------------------------------------------

        static const int MAX_DEPARTMENT_LENGTH = 12;

        char department[MAX_DEPARTMENT_LENGTH + 1];
};

#endif
