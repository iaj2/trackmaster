
/*

ProductRelease.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin and Isaac James
Rev. 2 - 2024/07/16 Added the Function Implementations 

--------------------------------------------------------------------------------------------------------------------

This module contains the class for the "Product" entity. This represents a specific release of a product.

This contains the following private data attributes:

Primary Attributes:
1. productName            (foreign key to "Product", "Request", and "Change" entity)
2. releaseID              (foreign key to "Change" entity)

Non-Primary Attributes:
3. releaseDate


This exported class contains public functions that allow the user to retrieve the attributes above as well as set the releaseID and releaseDate

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef PRODUCTRELEASE_H
#define PRODUCTRELEASE_H

#include "FileNotOpenException.h"
#include "FileOpenFailedException.h"
#include "RecordNotFoundException.h"

class ProductRelease {

    public:

    // --------------------------------------------------------------------------------------------------------------------

        /* Default constructor.

        Parameters: None

        Return: Returns a pointer to a product object.
        */

        ProductRelease();

    // --------------------------------------------------------------------------------------------------------------------

        /* Parameterized Constructor to create a "ProductRelease" Object. This requires the input of all private data attributes of the "ProductRelease" class.
    
        Parameter 1 (const char* productName): In parameter.
    
        Return: Does not return anything.
        */
        ProductRelease(const char* productName, int releaseID, int releaseDate);

    // --------------------------------------------------------------------------------------------------------------------
        
        /* Function to retrieve the "ProductRelease" object's productName.

        No parameters.

        Return: "ProductRelease" object's productName as an character array of maximum size 15.
        */
        char* getProductName() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "ProductRelease" object's releaseID.

        No parameters.

        Return: "ProductRelease" object's releaseID as a unique combination of integers.
        */
        int getReleaseID() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "ProductRelease" object's releaseDate.

        No parameters.

        Return: "ProductRelease" object's releaseDate as an 8-digit integer in the format of YYYY/MM/DD
        */
        int getReleaseDate() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "ProductRelease" object's releaseID.

        Parameter 1 (const int newReleaseID): In parameter.

        Return: Does not return anything.
        */
        void setReleaseID(const int newReleaseID);


    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "ProductRelease" object's releaseDate.

        Parameter 1 (const int newReleaseDate): In parameter.

        Return: Does not return anything.
        */
        void setReleaseDate(const int newReleaseDate);

        void setProductName(const char* newProductName);

    // --------------------------------------------------------------------------------------------------------------------

    private:
        static const int MAX_PRODUCT_NAME_LENGTH = 15;

        char productName[MAX_PRODUCT_NAME_LENGTH + 1];
        int releaseID;
        int releaseDate;
};

#endif
