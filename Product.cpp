#include "Product.h"
#include <iostream>
#include <cstring>
#include <fstream> 

using namespace std;

// Use static global file stream for product file operations
static std::fstream productFile;


// Default constructor
Product::Product() { }

// Initialize the product file
static void Product::initProduct() {
    // open the binary file 
    productFile.open("product.bat", std::ios::binary);
    if (!productFile) {
        // If the file does not exist, create it
        std::fstream { "product.bat" };
        // Open for reading and writing
        productFile.open("product.bat", std::ios::binary);
    }
    // if open does not work then throw an exception 
    if (!productFile) {
       throw FileOpenFailedException("File open failed");
    }
}

// Move the file pointer to the start of the file
static void Product::startOfProductFile() {
    // make sure the file is open 
    if (productFile.is_open()) {
        productFile.seekp(0, std::ios::beg); // Use seekg for reading positions
    } else {
        // if the file isnt open throw an exception 
        throw FileNotOpenException("File is not open");
    }
}

// Function to get a product record from the file
static Product* Product::getProductRecord() {
    // check if the file is open 
    if (productFile.is_open()) {

        // return nullptr if at end of file
        if (productFile.eof()) {
            return nullptr;
        }

        // create blank change object   
        Product* product = new Product();

        // read change record into object
        productFile.read(reinterpret_cast<char*>(product), sizeof(product));

        // if record not fully read, return nullptr
        if (productFile.fail() || productFile.eof()) {
            delete product;
            return nullptr;
        }
        return product; 
    // throw an exception if the file is not found 
    } else {
        throw FileNotOpenException("File is not open");
    }
}

// Record a new product at the end of the file
static void Product::recordProduct(const Product &newProduct) {
    // make sure the file is open 
    if (productFile.is_open()) {
        // seek to the end 
        productFile.seekp(0, std::ios::end);
        // create a product object instance 
        Product currentProduct(newProduct.getProductName());
        // write it to memory 
        productFile.write(reinterpret_cast<char*>(&currentProduct), sizeof(Product));
    // throw an exception if the file is not open 
    } else {
        throw FileNotOpenException("File is not open");
    }
}

// Report all products in the file
static void Product::reportAllProducts() {
    // make a count for display 
    int ProductCount = 1; 
    // make sure the file is open 
    if (productFile.is_open()) {
        // make an object instance
        Product currentProduct;
        // seek the begining of file 
        startOfProductFile();
        // display all the items 
        while (productFile.read(reinterpret_cast<char*>(&currentProduct), sizeof(Product))){
            std::cout << ProductCount << ") " << currentProduct.getProductName() << "\n"; 
            ProductCount++;
        }
    // throw an exception is the file isnt open
    } else {
        throw FileNotOpenException("File is not open");
    }
}

// Close the product file
static void Product::exitProduct() {
    // only close if the file is open 
    if (productFile.is_open()) {
        productFile.close();
    }
}

// Constructor with product name initialization
Product::Product(const char* newProductName) {
    this->setProductName(newProductName);
}

// Get the product name
static char* Product::getProductName() const {
    return const_cast<char*>(this->productName);
}


// Set the product name
void Product::setProductName(const char* newProductName) {
    strncpy(this->productName, newProductName, MAX_PRODUCT_NAME_LENGTH);
}