/*
Product.cpp
--------------------------------------------------------------------------------------------------------------------
Revision History:
Rev. 1 - 2024/07/13 Original by Bowen Jin
Rev. 2 - 2024/07/30 
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related to the header file 
--------------------------------------------------------------------------------------------------------------------
*/
#include "Product.h"
#include <iostream>
#include <cstring>
#include <fstream> 

using namespace std;

// -------------------------------------------------------------------------------------------------------------------
// Default Constructor 
// -------------------------------------------------------------------------------------------------------------------
Product::Product(){}

// -------------------------------------------------------------------------------------------------------------------
// Constructor with product name initialization
// -------------------------------------------------------------------------------------------------------------------
Product::Product(const char* newProductName) {
    this->setProductName(newProductName);
}

// -------------------------------------------------------------------------------------------------------------------
// Get the product name
// -------------------------------------------------------------------------------------------------------------------
char* Product::getProductName() const {
    return const_cast<char*>(this->productName);
}

// -------------------------------------------------------------------------------------------------------------------
// Set the product name
// Copies the new product name to the productName attribute, ensuring it is null-terminated.
// -------------------------------------------------------------------------------------------------------------------
void Product::setProductName(const char* newProductName) {
    strncpy(this->productName, newProductName, MAX_PRODUCT_NAME_LENGTH);
}
