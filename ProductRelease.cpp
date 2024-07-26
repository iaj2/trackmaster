#include "ProductRelease.h"
#include <iostream>
#include <cstring>
#include <fstream> 

using namespace std;

fstream ProductRelease::productReleaseFile;

// -------------------------------------------------------------------------------------------------------------------
// Default constructor
// -------------------------------------------------------------------------------------------------------------------
ProductRelease::ProductRelease() { }

// -------------------------------------------------------------------------------------------------------------------
// Parameterized Constructor to create a ProductRelease Object
// -------------------------------------------------------------------------------------------------------------------
ProductRelease::ProductRelease(const char* productName, int releaseID, int releaseDate){
    this->setProductName(productName);
    this->setReleaseDate(releaseDate);
    this->setReleaseID(releaseID);
}

// -------------------------------------------------------------------------------------------------------------------
// Retrieves the ProductRelease object's productName
// -------------------------------------------------------------------------------------------------------------------
char* ProductRelease::getProductName() const {
    return const_cast<char*>(this->productName);
}

// -------------------------------------------------------------------------------------------------------------------
// Retrieves the ProductRelease object's releaseID
// -------------------------------------------------------------------------------------------------------------------
int ProductRelease::getReleaseID() const {
    return releaseID;
}

// -------------------------------------------------------------------------------------------------------------------
// Retrieves the ProductRelease object's releaseDate
// -------------------------------------------------------------------------------------------------------------------
int ProductRelease::getReleaseDate() const {
    return releaseDate;
}//-------------------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------------------
// Sets the ProductRelease object's releaseID
// -------------------------------------------------------------------------------------------------------------------
void ProductRelease::setReleaseID(const int newReleaseID) {
    releaseID = newReleaseID;
}

// -------------------------------------------------------------------------------------------------------------------
// Sets the ProductRelease object's releaseDate
// -------------------------------------------------------------------------------------------------------------------
void ProductRelease::setReleaseDate(const int newReleaseDate) {
    releaseDate = newReleaseDate;
}

// -------------------------------------------------------------------------------------------------------------------
// Sets the ProductRelease object's productName
// Copies the new product name to the productName attribute, ensuring it is null-terminated.
// -------------------------------------------------------------------------------------------------------------------
void ProductRelease::setProductName(const char* newProductName){
    strncpy(this->productName, newProductName, MAX_PRODUCT_NAME_LENGTH);
}
