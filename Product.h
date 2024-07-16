/*

Product.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin and Isaac James
Rev. 2 - 2024/07/16 Added function implementations 

--------------------------------------------------------------------------------------------------------------------

This module contains the class for the "Product" entity. This is designed to encapsulate data and operations related to a single "Product". This contains the following private data attributes:

Primary Attributes:
1. productName            (foreign key to "ProductRelease" and "Change" entity)


This exported class contains public functions that allow the user to:
    1. Initialize
    2. Record
    3. Report
Products.

The class restricts direct access to the private "productName" attribute, ensuring only controlled modification via private setter methods.

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef PRODUCT_H
#define PRODUCT_H

#include "FileNotOpenException.h"
#include "FileOpenFailedException.h"
#include "RecordNotFoundException.h"

class Product {

    public:
    // -------------------------------------------------------------------------------------------------------------------

        /* Default constructor for Product Object 

        Parameters: None
    
        Return: Pointer to product Object.
        */
        Product();

    // --------------------------------------------------------------------------------------------------------------------

        /* Opens the necessary files relevant for Product records.
    
        Parameters: None
    
        Return: Does not return anything.
        */
        static void initProduct();

    // --------------------------------------------------------------------------------------------------------------------

        /* Seeks to start of the product file.
    
        Parameters: None
    
        Return: Does not return anything.
    
        */
        static void startOfProductFile();

    // --------------------------------------------------------------------------------------------------------------------

        /* Gets product record currently pointed to in file.
    
        Parameters: None
    
        Return: reference to the product record or nullptr if end of file.
    
        */
        static Product* getProductRecord();
    // --------------------------------------------------------------------------------------------------------------------

        /* Takes in the created "Product" object to write to file.
        
        Parameters: Product newProduct. In parameter.
        
        Return: Does not return anything.
        */
        static void recordProduct(const Product &newProduct);

    // --------------------------------------------------------------------------------------------------------------------

        /* Generates a report for all products.
    
        Parameters: None
    
        Return: Does not return anything.
        */
        static void reportAllProducts();

    // --------------------------------------------------------------------------------------------------------------------

        /* Exits the Product Operation.
    
        Parameters: None
    
        Return: Does not return anything.
        */
        static void exitProduct();

    // --------------------------------------------------------------------------------------------------------------------

        /* Parameterized Constructor to create a "Product" Object. This requires the input of all private data attributes of the "Product" class.
        
        Parameter 1 (const char* productName): In parameter.
        
        Return: Does not return anything.
        */
        Product(const char* productName);

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to retrieve the "Product" object's productName.

        No parameters.

        Return: "Product" object's productName as an character array of maximum size 15.
        */
        char* getProductName() const;

    // --------------------------------------------------------------------------------------------------------------------

        /* Function to set the "Product" object's productName.

        Parameter 1 (const char* newProductName): In parameter.

        Return: Does not return anything.
        */
        void setProductName(const char* newProductName);

    // --------------------------------------------------------------------------------------------------------------------

     

    private:
    
        static const int MAX_PRODUCT_NAME_LENGTH = 15;
        char productName[MAX_PRODUCT_NAME_LENGTH + 1];


};

#endif