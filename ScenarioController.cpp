#include "ScenarioController.h"
#include "PrintController.h"
#include "Change.h"
#include "Requester.h"
#include "Product.h"
#include "ProductRelease.h"
#include "UI.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;
using namespace UI;

void clearScreen() {
    system("clear");
}

Requester** getNRequestRecords(const int n) {
    int i = 0;
    Requester* currentRequester = nullptr;
    Requester** requesters = new Requester*[n]; // Allocate array of Requester* pointers

    while (i < n && (currentRequester = Requester::getRequesterRecord()) != nullptr) {
        requesters[i] = currentRequester;
        i++;
    }

    return requesters;
}

string formatSelectionRange(int start, int end) {
    string range = "[" + to_string(start) + "-" + to_string(end) + "]";
    return range;
}

void ScenarioController::createRequestControl() {
    // get requester type from user
    char reqTSelection;
    do {
        cout << "Is this request coming from a customer or an employee?" << endl;
        cout << "ENTER a selection [c - customer/e - employee]" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu" << endl;

        reqTSelection = cin.get();
        // exit operation
        if (reqTSelection == '0') {
            return;
        }
        // invalid input
        else if (reqTSelection != 'c' && reqTSelection != 'e') {
            // clear screen between attempts
            clearScreen();
           
            cout << "Error: Input is invalid. Re-enter input" << endl;
            cout << "Enter 0 to abort and return to the main menu" << endl << endl;
        }

        // Clear stdin
        cin.ignore();

    } while(reqTSelection != 'c' && reqTSelection != 'e');

    // clear screen between prompts
    clearScreen();      

    // get requester from user
    const int maxRecordOutput = 5;
    int recordOffset = 0;
    string range;
    // seek to start of file and get first set of items
    Requester::startOfRequesterFile();
    Requester** requesters = getNRequestRecords(maxRecordOutput);
    string requesterSelection;
    int option;
    do {
        range = formatSelectionRange(1, Requester::getRequesterCount());
        cout << "=== Select Customer ===" << endl;
        // list items
        for(int i = 0; i < maxRecordOutput; i++) {
            cout << " Name" << "        Email" << endl;
            cout << to_string(i) << ")";
            if (requesters[i] == nullptr) {
                cout << "Record unavailable" << endl;
            }
            else {
                const char * name = requesters[i]->getName();
                const char * email = requesters[i]->getRequesterEmail();
                cout << name << "        " << email << endl;
            }
        }
        // if list of records too long
        if (maxRecordOutput < Requester::getRequesterCount()) {
            cout << "*..." << endl;
            if (recordOffset > maxRecordOutput) {
            cout << "p) display previous items" << endl;
            // seek
            }
            cout << "n) display next items" << endl;
        }
        cout << "ENTER selection " << range << " OR <0> to abort and exit to the main menu:";
        
        getline(cin, requesterSelection);

        try {
            // Attempt to convert string to integer using stoi
            option = stoi(requesterSelection);

            // check for exit
            if (option == 0) return;

            // check selected valid option
            if(option > 0 && option <= Requester::getRequesterCount()) {
                break;
            }
            // check for display next
            else {
                // clear screen between attempts
                clearScreen();
                cout << "Error: The option you entered does not exist on the list" << endl << endl;
            }

        } catch (const exception& e) {
            // clear screen between attempts
            if (requesterSelection == "p" && recordOffset - maxRecordOutput >= 0) {
                // move pointer and get previous set of items
                recordOffset -= maxRecordOutput;
                Requester::seekRequesterFile(recordOffset);
                requesters = getNRequestRecords(maxRecordOutput);
            }
            else if (requesterSelection == "n" && recordOffset + maxRecordOutput <= Requester::getRequesterCount()) {
                // move pointer and get next set of items
                recordOffset += maxRecordOutput;
                Requester::seekRequesterFile(recordOffset);
                requesters = getNRequestRecords(maxRecordOutput);
            }
            else {
                cout << "Error: Input is invalid. Re-enter input" << endl;
                cout << "Enter 0 to abort and return to the main menu" << endl << endl;
            }
            clearScreen();
        }

    } while (true);
    
}

void ScenarioController::createRequesterControl() {
    clearScreen();

    // Step 3: Get the email address of the requester
    char requesterEmail[Requester::MAX_EMAIL_LENGTH + 1];
    do {
        cout << "ENTER the EMAIL ADDRESS of the requester (Length: max 30)" << endl;
        cout << "OR ENTER <0> to abort and go back to the main menu: ";
        cin.getline(requesterEmail, Requester::MAX_EMAIL_LENGTH + 1);

        if (strcmp(requesterEmail, "0") == 0) {
            return;
        } else if (strlen(requesterEmail) > Requester::MAX_EMAIL_LENGTH) {
            clearScreen();
            cout << "Error: Email address is too long. Please re-enter." << endl << endl;
        }
    } while (strlen(requesterEmail) > Requester::MAX_EMAIL_LENGTH);

    clearScreen();

    // Step 4: Get the phone number of the requester
    int phone;
    string phoneInput;
    do {
        cout << "ENTER the PHONE NUMBER of the requester (Length: Exactly 10)" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";
        getline(cin, phoneInput);

        if (phoneInput == "0") {
            return;
        } else if (phoneInput.length() != 10) {
            clearScreen();
            cout << "Error: Phone number must be exactly 10 digits. Please re-enter." << endl << endl;
        } else {
            phone = stoi(phoneInput);
        }
    } while (phoneInput.length() != 10);

    clearScreen();

    // Step 5: Get the last name followed by the first name of the requester
    char name[Requester::MAX_NAME_LENGTH + 1];
    do {
        cout << "ENTER the LAST NAME followed by the FIRST NAME of the requester (Length: max 15)" << endl;
        cout << "OR ENTER <0> to exit to the main menu: ";
        cin.getline(name, Requester::MAX_NAME_LENGTH + 1);

        if (strcmp(name, "0") == 0) {
            return;
        } else if (strlen(name) > Requester::MAX_NAME_LENGTH) {
            clearScreen();
            cout << "Error: Name is too long. Please re-enter." << endl << endl;
        }
    } while (strlen(name) > Requester::MAX_NAME_LENGTH);

    clearScreen();

    // Step 6: Get the department name of the requester
    char department[Requester::MAX_DEPARTMENT_LENGTH + 1];
    do {
        cout << "ENTER the DEPARTMENT NAME of the requester (Length: max 12)" << endl;
        cout << "***Must be left blank if requester is not an employee of the company***" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";
        cin.getline(department, Requester::MAX_DEPARTMENT_LENGTH + 1);

        if (strcmp(department, "0") == 0) {
            return;
        } else if (strlen(department) > Requester::MAX_DEPARTMENT_LENGTH) {
            clearScreen();
            cout << "Error: Department name is too long. Please re-enter." << endl << endl;
        }
    } while (strlen(department) > Requester::MAX_DEPARTMENT_LENGTH);

    

    clearScreen();

   // Create the requester object
    Requester* newRequester = new Requester (requesterEmail,  name,  phone, department);
    Requester::recordRequester(*newRequester);
    delete newRequester;


    // Step 7: Confirmation message
    cout << "The new requester has been successfully added to the system." << endl;
    cout << "ENTER <0> to go back to the main menu: ";
    string input;
    do {
        getline(cin, input);
    } while (input != "0");
}

void ScenarioController::createProductControl() {
    clearScreen();

    // Step 3: Get the product name
    char productName[Product::MAX_PRODUCT_NAME_LENGTH + 1];
    do {
        cout << "ENTER the PRODUCT NAME (Length: max 10)" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";
        cin.getline(productName, Product::MAX_PRODUCT_NAME_LENGTH + 1);

        if (strcmp(productName, "0") == 0) {
            return; // Abort and return to main menu
        } else if (strlen(productName) > Product::MAX_PRODUCT_NAME_LENGTH) {
            clearScreen();
            cout << "Error: Product name is too long. Please re-enter." << endl << endl;
        }
    } while (strlen(productName) > Product::MAX_PRODUCT_NAME_LENGTH);

    // Create the product object
    Product* newProduct = new Product(productName);
    Product::recordProduct(*newProduct);
    delete newProduct; // Free allocated memory for the product object

    clearScreen();

    // Step 4: Confirmation message
    cout << "The new product has been successfully added to the system." << endl;
    cout << "ENTER <0> to go back to the main menu: ";
    string input;
    do {
        getline(cin, input);
    } while (input != "0");

}

Product** getProductRecords(const int n) {
    int i = 0;
    Product* currentProduct = nullptr;
    Product** Products = new Product*[n]; // Allocate array of Requester* pointers

    while (i < n && (currentProduct = Product::getProductRecord()) != nullptr) {
        Products[i] = currentProduct;
        i++;
    }

    return Products;
}

Change** getChangeRecords(const int n) {
    int i = 0;
    Change* currentChange = nullptr;
    Change** Changes = new Change*[n]; // Allocate array of Requester* pointers

    while (i < n && (currentChange = Change::getChangeRecord()) != nullptr) {
        Changes[i] = currentChange;
        i++;
    }

    return Changes;
}

void inquireChangeItem() {
    clearScreen();

    // Step 1: Enter Inquire Menu
    cout << "=== Inquire Menu ===" << endl;

    // Get product count
    int productCount = Product::getProductCount();

    if (productCount == 0) {
        cout << "No products available for inquiry." << endl;
        return;
    }

    // Allocate array of Product pointers dynamically
    Product** products = getProductRecords(productCount);

    if (!products) {
        cout << "Error: Failed to retrieve product records." << endl;
        return;
    }

    cout << "Select the product you wish to inquire a change item from:" << endl;

    for (int i = 0; i < productCount; ++i) {
        if (products[i] != nullptr) {
            cout << i + 1 << ") " << products[i]->getProductName() << endl;
        }
    }
    cout << "0) Exit" << endl;

    int productSelection;
    string range;
    do {
        range = formatSelectionRange(0, productCount);
        cout << "ENTER Selection " << range << ": ";
        cin >> productSelection;

        if (productSelection == 0) {
            clearScreen();
            return; // Exit option
        } else if (productSelection < 0 || productSelection > productCount) {
            clearScreen();
            cout << "Error: Invalid selection. Please enter a number between 0 and " << productCount << "." << endl;
        }

    } while (productSelection < 0 || productSelection > productCount);

    clearScreen();

    // Step 2: Select a change item to inquire
    cout << "=== Select Change Item ===" << endl;
    // Assuming Change class has a method to get change records
    int maxChanges = Change::getChangeCount(); // Example: Retrieve 2 change items
    Change** changes = getChangeRecords(maxChanges);
    
    cout << "ENTER selection [1-" << maxChanges << "] OR <0> to abort and exit to main menu: ";

    int changeSelection;
    do {
        cin >> changeSelection;

        if (changeSelection == 0) {
            clearScreen();
            return; // Abort and exit to main menu
        } else if (changeSelection < 1 || changeSelection > maxChanges) {
            clearScreen();
            cout << "Error: Invalid selection. Please enter a number between 1 and " << maxChanges << "." << endl;
            cout << "ENTER selection [1-" << maxChanges << "] OR <0> to abort and exit to main menu: ";
        }

    } while (changeSelection < 1 || changeSelection > maxChanges);

    clearScreen();

    // Step 3: Display change item details
    cout << "Change Item Report:" << endl;
    cout << "Product name: " << products[productSelection - 1]->getProductName() << endl; // Display selected product name
    cout << "ChangeID: " << changes[changeSelection - 1]->getchangeID() << endl; // Assuming Change class methods for details
    cout << "First Reported: " << changes[changeSelection - 1]->getDate() << endl;
    cout << "Status: " << Change::statusToString(changes[changeSelection - 1]->getStatus()) << endl;
    cout << "Anticipated release: " << changes[changeSelection - 1]->getAnticipatedReleaseID() << endl;
    cout << "Description: " << changes[changeSelection - 1]->getDescription() << endl << endl;

    // Step 4: Ask for further inquiry or return to main menu
    cout << "Enter <1> to inquire about another change item." << endl;
    cout << "Enter <0> to go back to the main menu." << endl;
    cout << "ENTER Selection: ";

    int continueOption;
    do {
        cin >> continueOption;

        if (continueOption == 0) {
            clearScreen();
            return; // Return to main menu
        } else if (continueOption != 1) {
            clearScreen();
            cout << "Error: Invalid input. Please enter 0 or 1." << endl;
            cout << "Enter <1> to inquire about another change item." << endl;
            cout << "Enter <0> to go back to the main menu." << endl;
            cout << "ENTER Selection: ";
        }

    } while (continueOption != 0 && continueOption != 1);

    clearScreen();
}


ProductRelease** getProductReleaseRecords(const int n) {
    int i = 0;
    ProductRelease* currentProductRelease = nullptr;
    ProductRelease** ProductReleases = new ProductRelease*[n]; // Allocate array of Requester* pointers

    while (i < n && (currentProductRelease = ProductRelease::getProductReleaseRecord()) != nullptr) {
        ProductReleases[i] = currentProductRelease;
        i++;
    }

    return ProductReleases;
}


// Updated printScenario1Control using getProductReleaseRecords and getProductRecords
void printScenario1Control() {
    int productSelection;
    int releaseSelection;

    // Get product records
    int Productscount = Product::getProductCount();
    Product** products = getProductRecords(Productscount); // Assuming 4 products
    if (!products) {
        cout << "Error fetching product records. Aborting." << endl;
        return;
    }

    cout << "\n=== Select Product ===" << endl;
    for (int i = 0; i < Productscount; ++i) {
        cout << i + 1 << ") " << products[i]->getProductName() << endl;
    }
    cout << "ENTER Selection [1-4] OR 0 to abort and exit to the main menu: ";
    cin >> productSelection;

    if (productSelection == 0) {
        delete[] products;
        return;
    }

    // Validate user input
    if (productSelection < 1 || productSelection > 4) {
        cout << "Invalid selection. Aborting." << endl;
        delete[] products;
        return;
    }

    // Get product release records
    ProductRelease** releases = getProductReleaseRecords(4); // Assuming 4 releases
    if (!releases) {
        cout << "Error fetching product release records. Aborting." << endl;
        delete[] products;
        return;
    }

    cout << "\n=== Select Product Release ===" << endl;
    for (int i = 0; i < 4; ++i) {
        cout << i + 1 << ") " << releases[i]->getReleaseID() << "." << releases[i]->getReleaseID() << endl;
    }
    cout << "ENTER Selection [1-4] OR 0 to abort and exit to the main menu: ";
    cin >> releaseSelection;

    if (releaseSelection == 0) {
        delete[] products;
        delete[] releases;
        return;
    }

    // Validate user input
    if (releaseSelection < 1 || releaseSelection > 4) {
        cout << "Invalid selection. Aborting." << endl;
        delete[] products;
        delete[] releases;
        return;
    }

    // Set up selected product and release
    Product selectedProduct = *products[productSelection - 1];
    ProductRelease selectedRelease = *releases[releaseSelection - 1];

    // Call PrintController method with selected product and release
    PrintController::printProduct(selectedRelease);

    // Clean up allocated memory
    delete[] products;
    delete[] releases;
}

// Updated printScenario2Control using getProductRecords
void printScenario2Control() {
    int productSelection;

    // Get product records
    int Productscount = Product::getProductCount();
    Product** products = getProductRecords(Productscount); // Assuming 4 products
    if (!products) {
        cout << "Error fetching product records. Aborting." << endl;
        return;
    }

    cout << "\n=== Select Product ===" << endl;
    for (int i = 0; i < Productscount; ++i) {
        cout << i + 1 << ") " << products[i]->getProductName() << endl;
    }
    cout << "ENTER Selection [1-4] OR 0 to abort and exit to the main menu: ";
    cin >> productSelection;

    if (productSelection == 0) {
        delete[] products;
        return;
    }

    // Validate user input
    if (productSelection < 1 || productSelection > 4) {
        cout << "Invalid selection. Aborting." << endl;
        delete[] products;
        return;
    }

    // Clean up allocated memory
    delete[] products;
}


// REMOVE
int main () {
    Requester::initRequester();
    ScenarioController::createRequestControl();
    cout << "EXITED" << endl;
    return 0;
}

