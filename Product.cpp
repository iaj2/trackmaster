#include "Product.h"
#include <iostream>
#include <cstring>
#include <fstream> 

using namespace std;

// Use static file stream for product file operations
fstream Product::productFile;

int Product::productCount = 0;

// -------------------------------------------------------------------------------------------------------------------
// Default constructor
// -------------------------------------------------------------------------------------------------------------------
Product::Product() { }

// -------------------------------------------------------------------------------------------------------------------
// Initialize the product file
// Opens the product file or creates it if it doesn't exist.
// -------------------------------------------------------------------------------------------------------------------
void Product::initProduct() {
    // Open the binary file
    productFile.open("product.bat", std::ios::binary);
    if (!productFile) {
        // If the file does not exist, create it
        std::fstream { "product.bat" };
        // Open for reading and writing
        productFile.open("product.bat", std::ios::binary);
    }
    // If open does not work then throw an exception
    if (!productFile) {
       throw FileOpenFailedException("Product file open failed");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Move the file pointer to the start of the file
// -------------------------------------------------------------------------------------------------------------------
void Product::startOfProductFile() {
    // Make sure the file is open
    if (productFile.is_open()) {
        productFile.seekp(0, std::ios::beg); // Use seekg for reading positions
    } else {
        // if the file isnt open throw an exception 
        throw FileNotOpenException("Product file is not open");
        // If the file isn't open, throw an exception
        throw FileNotOpenException("File is not open");
    }
}

// ------------------------------------------------------------------------------------------------
// Move the product file pointer to an offset from the start
// -----------------------------------------------------------------------------------------------
void Product::seekProductFile(int records_offset) {
    if (productFile.is_open()) {
        productFile.seekg(sizeof(Product)*records_offset, ios::beg);
    } else {
        // if the file isnt open throw an exception 
        throw FileNotOpenException("Product file is not open");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Method to get a product record from the file
// Reads a product record from the current file pointer position.
// -------------------------------------------------------------------------------------------------------------------
Product* Product::getProductRecord() {
    // Check if the file is open
    if (productFile.is_open()) {
        // Return nullptr if at end of file
        if (productFile.eof()) {
            return nullptr;
        }

        // Create blank product object   
        Product* product = new Product();

        // Read product record into object
        productFile.read(reinterpret_cast<char*>(product), sizeof(product));

        // If record not fully read, return nullptr
        if (productFile.fail() || productFile.eof()) {
            delete product;
            return nullptr;
        }
        return product; 
    } else {
        throw FileNotOpenException("Product file is not open");
        // Throw an exception if the file is not open
        throw FileNotOpenException("File is not open");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Record a new product at the end of the file
// -------------------------------------------------------------------------------------------------------------------
const void Product::recordProduct(Product &newProduct) {
    // Make sure the file is open
    if (productFile.is_open()) {
        // Seek to the end 
        productFile.seekp(0, std::ios::end);

        // write it to file 
        productFile.write(reinterpret_cast<char*>(&newProduct), sizeof(Product));
        
        // increase count
        productCount++;
    } else {
        throw FileNotOpenException("Product file is not open");
        // Throw an exception if the file is not open
        throw FileNotOpenException("File is not open");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Report all products in the file
// Reads and displays all products from the file.
// -------------------------------------------------------------------------------------------------------------------
void Product::reportAllProducts() {
    // Make a count for display 
    int ProductCount = 1; 
    // Make sure the file is open 
    if (productFile.is_open()) {
        // Make an object instance
        Product currentProduct;
        // Seek the beginning of file 
        startOfProductFile();
        // Display all the items 
        while (productFile.read(reinterpret_cast<char*>(&currentProduct), sizeof(Product))) {
            std::cout << ProductCount << ") " << currentProduct.getProductName() << "\n"; 
            ProductCount++;
        }
    } else {
        // Throw an exception if the file isn't open
        throw FileNotOpenException("File is not open");
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Close the product file
// -------------------------------------------------------------------------------------------------------------------
void Product::exitProduct() {
    // Only close if the file is open 
    if (productFile.is_open()) {
        productFile.close();
    }
}

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
// Get the product name
// -------------------------------------------------------------------------------------------------------------------
 int Product::getProductCount() {
    return productCount;
}

// -------------------------------------------------------------------------------------------------------------------
// Set the product name
// Copies the new product name to the productName attribute, ensuring it is null-terminated.
// -------------------------------------------------------------------------------------------------------------------
void Product::setProductName(const char* newProductName) {
    strncpy(this->productName, newProductName, MAX_PRODUCT_NAME_LENGTH);
}
