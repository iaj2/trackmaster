#include "ProductRelease.h"
#include <iostream>
#include <cstring>
#include <fstream> 

using namespace std;

int ProductRelease::productRelCount = 0;
fstream ProductRelease::productReleaseFile;

// -------------------------------------------------------------------------------------------------------------------
// Default constructor
// -------------------------------------------------------------------------------------------------------------------
ProductRelease::ProductRelease() { }

// -------------------------------------------------------------------------------------------------------------------
// Initializes the ProductRelease files
// Opens the product release file or creates it if it doesn't exist.
// -------------------------------------------------------------------------------------------------------------------
void ProductRelease::initProductRelease() {
    // Open the binary file
    productReleaseFile.open("product_release.bin", std::ios::binary);
    if (!productReleaseFile) {
        // If the file does not exist, create it
        std::ofstream { "product_release.bin" };
        // Open for reading and writing
        productReleaseFile.open("product_release.bin", std::ios::binary);
    }
    // If open does not work then throw an exception
    if (!productReleaseFile) {
       throw FileOpenFailedException("File open failed");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Move the file pointer to the start of the file
// -------------------------------------------------------------------------------------------------------------------
void ProductRelease::startOfProductReleaseFile() {
    // Make sure the file is open
    if (productReleaseFile.is_open()) {
        productReleaseFile.seekg(0, std::ios::beg);
    } else {
        // If the file isn't open, throw an exception
        throw FileNotOpenException("File is not open");
    }
}

// ------------------------------------------------------------------------------------------------
// Move the productRelease file pointer to an offset from the start
// -----------------------------------------------------------------------------------------------
void ProductRelease::seekProductRelFile(int records_offset) {
    if (productReleaseFile.is_open()) {
        productReleaseFile.seekg(sizeof(ProductRelease)*records_offset, ios::beg);
    } else {
        // if the file isnt open throw an exception 
        throw FileNotOpenException("Product release file is not open");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Function to get a product release record from the file
// Reads a product release record from the current file pointer position.
// -------------------------------------------------------------------------------------------------------------------
ProductRelease* ProductRelease::getProductReleaseRecord() {
    // Check if the file is open
    if (productReleaseFile.is_open()) {
        // Return nullptr if at end of file
        if (productReleaseFile.eof()) {
            return nullptr;
        }

        // Create blank product release object
        ProductRelease* productRelease = new ProductRelease();

        // Read product release record into object
        productReleaseFile.read(reinterpret_cast<char*>(productRelease), sizeof(productRelease));

        // If record not fully read, return nullptr
        if (productReleaseFile.fail() || productReleaseFile.eof()) {
            delete productRelease;
            return nullptr;
        }
        return productRelease; 
    } else {
        // Throw an exception if the file is not open
        throw FileNotOpenException("File is not open");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Records a new ProductRelease object to the file
// -------------------------------------------------------------------------------------------------------------------
void ProductRelease::recordProductRelease(const ProductRelease& newProductRelease) {
    // Make sure the file is open
    if (productReleaseFile.is_open()) {
        // Seek to the end
        productReleaseFile.seekp(0, std::ios::end);
        // Create a product release object instance
        ProductRelease currentProductRelease(newProductRelease.getProductName(), newProductRelease.getReleaseID(), newProductRelease.getReleaseDate());
        // Write it to memory
        productReleaseFile.write(reinterpret_cast<char*>(&currentProductRelease), sizeof(ProductRelease));
        // increase count
        productRelCount++;
    } else {
        // Throw an exception if the file is not open
        throw FileNotOpenException("File is not open");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Generates a report for a specific product release
// Reads and displays product releases that match the specified product name.
// -------------------------------------------------------------------------------------------------------------------
void ProductRelease::reportProductRelease(const ProductRelease& newProductRelease) {
    // Make a counter for presentation
    int ProductReleaseCount = 1; 
    // Make sure the file is open
    if (productReleaseFile.is_open()) {
        // Make an object instance to record the information in
        ProductRelease currentProductRelease;
        // Go to the beginning of file
        startOfProductReleaseFile();
        // Read from the file in chunks
        while (productReleaseFile.read(reinterpret_cast<char*>(&currentProductRelease), sizeof(ProductRelease))) {
            // If the product names match, print to the screen
            if(currentProductRelease.productName == newProductRelease.productName){
                cout << ProductReleaseCount << ") " << currentProductRelease.releaseID << "\n"; 
                ProductReleaseCount++;
            }
        }
    } else {
        // Throw an exception if the file is not open
        throw FileNotOpenException("File is not open");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Exits the ProductRelease operation
// -------------------------------------------------------------------------------------------------------------------
void ProductRelease::exitProductRelease() {
    // If the file is open, close it
    if (productReleaseFile.is_open()) {
        productReleaseFile.close();
    }
}

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
// Function to retrieve the "Requester" object's record count.
// -------------------------------------------------------------------------------------------------------------------
int ProductRelease::getProductRelCount() {
    return productRelCount;
}

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
