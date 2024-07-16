
/*

ProductRelease.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin and Isaac James

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

class ProductRelease {

    public:

    // --------------------------------------------------------------------------------------------------------------------

        /* Opens the necessary files relevant for "ProductRelease" records.

        Parameters: None

        Return: Does not return anything.
        */
        static void initProductRelease();

    // --------------------------------------------------------------------------------------------------------------------

        /* Seeks to start of the product release file.

        Parameters: None

        Return: Does not return anything.

        */
        static void startOfProductReleaseFile();

    // --------------------------------------------------------------------------------------------------------------------

        /* Gets product release record currently pointed to in file.

        Parameters: None

        Return: reference to the product release record or nullptr if end of file.

        */
        static ProductRelease* getProductReleaseRecord();
    // --------------------------------------------------------------------------------------------------------------------

        /* Takes in the created "ProductRelease" object to write to file.
        
        Parameters (ProductRelease newProductRelease): In parameter.
        
        Return: Does not return anything.
        */
        static void recordProductRelease(ProductRelease newProductRelease);

    // --------------------------------------------------------------------------------------------------------------------

    /* Generates a report for a specific product release.

        Parameters: None

        Return: Does not return anything.
        */
        void reportProductRelease();

    // --------------------------------------------------------------------------------------------------------------------

    /* Exits the ProductRelease Operation.

        Parameters: None

        Return: Does not return anything.
        */
        static void exitProductRelease();

    // --------------------------------------------------------------------------------------------------------------------

        /* Parameterized Constructor to create a "ProductRelease" Object. This requires the input of all private data attributes of the "ProductRelease" class.
    
        Parameter 1 (const char* productName): In parameter.
    
        Return: Does not return anything.
        */
        ProductRelease(const char* productName, int releaseID, int releaseDate);
    
    // --------------------------------------------------------------------------------------------------------------------
    
    private:

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

    // --------------------------------------------------------------------------------------------------------------------

    private:
        static const int MAX_PRODUCT_NAME_LENGTH = 15;

        char productName[MAX_PRODUCT_NAME_LENGTH + 1];
        int releaseID;
        int releaseDate;
};

#endif
