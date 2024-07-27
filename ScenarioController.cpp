#include "ScenarioController.h"
#include "PrintController.h"
#include "Change.h"
#include "Requester.h"
#include "Product.h"
#include "Request.h"
#include "ProductRelease.h"
#include "UI.h"
#include "EntityIO.h"
#include "globalEntities.h"
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>

using namespace std;
using namespace UI;

// Some global constants
const int maxRecordOutput = 5; 

const vector<string> customerColHeaders = {"Name", "Email"};
const vector<string> employeeColHeaders = {"Name", "Email", "Department"};
const vector<string> changeColHeaders = {"Description", "ChangeID"};
const vector<string> openChangeColHeaders = {"Product", "Description", "Status"};
const vector<string> assessedChangeColHeaders = {"Description", "Status", "ChangeID"};
const vector<string> noColHeader = {""};

// Some helper functions

void clearScreen() {
    system("clear");
}

string formatSelectionRange(int start, int end) {
    return "[" + to_string(start) + "-" + to_string(end) + "]";
}

void clearScreenAndShowError(const string& errorMessage) {
    clearScreen();
    cout << "Error: " << errorMessage << " Please re-enter." << endl << endl;
}

string getInput(const string& prompt, int maxLength) {
    string input;
    do {
        cout << prompt << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";
        cin >> input; 
        cin.clear();
        cin.ignore(10000,'\n');

        if (input == "0") {
            return "0";
        } else if (input.length() > maxLength) {
            clearScreenAndShowError("Input is too long.");
        }
    } while (input.length() > maxLength);
    return input;
}


string getEmail() {
    return getInput("ENTER the EMAIL ADDRESS of the requester (Length: max 30)", Requester::MAX_EMAIL_LENGTH);
}

string getPhone() {
    string phoneInput;
    do {
        cout << "ENTER the PHONE NUMBER of the requester (Length: Exactly 10)" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";
        cin >> phoneInput;
        cin.clear();
        cin.ignore(10000,'\n');
        if (phoneInput == "0") {
            return "0";
        } else if (phoneInput.length() != 10) {
            clearScreenAndShowError("Phone number must be exactly 10 digits.");
        }
    } while (phoneInput.length() != 10);
    return phoneInput;
}

string getName() {
    return getInput("ENTER the LAST NAME followed by the FIRST NAME of the requester (Length: max 15)", Requester::MAX_NAME_LENGTH);
}

string getDepartment() {
    return getInput("ENTER the DEPARTMENT NAME of the requester (Length: max 12)\n***Must be left blank if requester is not an employee of the company***", Requester::MAX_DEPARTMENT_LENGTH);
}


string getProductName() {
    return getInput("ENTER the PRODUCT NAME (Length: max 10)", Product::MAX_PRODUCT_NAME_LENGTH);
}

string getReleaseIDData() {
    string releaseIDInput;
    do {
        cout << "ENTER the release ID of the Product Release (integrs, Length: at most 4)" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";
        cin >> releaseIDInput;
        cin.clear();
        cin.ignore(10000,'\n');
        if (releaseIDInput == "0") {
            return "0";
        } else if (releaseIDInput.length() > 4) {
            clearScreenAndShowError(".");
        }
    } while (releaseIDInput.length() > 4);
    return releaseIDInput;
}

string getDateData() {
    string dateInput;
    do {
        cout << "ENTER the DATE of the request (integers YYYYMMDD) OR ENTER <0> to abort and" << endl;
        cout << "exit to the main menu:";
        cin >> dateInput;
        cin.clear();
        cin.ignore(10000,'\n');
        if (dateInput == "0") {
            return "0";
        } else if (dateInput.length() > 8) {
            clearScreenAndShowError(".");
        }
    } while (dateInput.length() > 8);
    return dateInput;
}

void printCustomerRow(Requester& requester) {
    cout << requester.getName() << "   " << requester.getRequesterEmail();
}

void printEmployeeRow(Requester& requester) {
    cout << requester.getName() << "   " << requester.getRequesterEmail() << "   " << requester.getDepartment();
}

void printProductRow(Product& product) {
    cout << product.getProductName();
}

void printProductReleaseRow(ProductRelease& productRelease) {
    cout << to_string(productRelease.getReleaseID());
}

void printChangeRow(Change& change) {
    cout << change.getDescription() << "   " << to_string(change.getchangeID());
}

void printOpenChangeRow(Change& change) {
    cout << change.getProductName() << "   " << change.getDescription() << "   " << Change::statusToString(change.getStatus());
}

void printAssessedChangeRow(Change& change) {
    cout << change.getDescription() << "   " << Change::statusToString(change.getStatus()) << "   " << to_string(change.getchangeID());
}

template<typename T>
T* selectFromList(EntityIO<T>& entityIO, const string& title, const vector<string>& columnNames, void (*printRow)(T &entity)) {
    int recordOffset = 0;
    entityIO.seekToStart();
    vector<T*> records = entityIO.readNRecords(maxRecordOutput);
    string selection;
    int option;
    string range;

    while (true) {
        range = formatSelectionRange(1, entityIO.getRecordCount());
        cout << "=== Select " << title << " ===" << endl;

        // Print column headers
        if(columnNames[0] != "") {
            for (const string& columnName : columnNames) {
                        cout << columnName << "   ";
                    }
            cout << endl;
        }
        
        // print rows
        for (int i = 0; i < maxRecordOutput; ++i) {
            cout << to_string(recordOffset + i + 1) << ")";
            if (records[i] == nullptr) {
                cout << "Record unavailable" << endl;
            } else {
                printRow(*records[i]);
                cout << endl;
            }
        }

        // "scrolling" functionality
        if (maxRecordOutput < entityIO.getRecordCount()) {
            cout << "*..." << endl;
            if (recordOffset > maxRecordOutput) {
                cout << "p) display previous items" << endl;
            }
            cout << "n) display next items" << endl;
        }
        
        cout << "ENTER selection " << range << " OR <0> to abort and exit to the main menu:";

        getline(cin, selection);

        try {
            option = stoi(selection);
            if (option == 0) return nullptr;

            if (option > 0 && option <= entityIO.getRecordCount()) {
                T* selectedRecord = records[option - recordOffset - 1];
                for (T* record : records) delete record;  // Free memory
                return selectedRecord;
            } else {
                clearScreen();
                cout << "Error: The option you entered does not exist on the list" << endl << endl;
            }

        } catch (const exception&) {
            if (selection == "p" && recordOffset - maxRecordOutput >= 0) {
                recordOffset -= maxRecordOutput;
                entityIO.seekTo(recordOffset);
                records = entityIO.readNRecords(maxRecordOutput);
            } else if (selection == "n" && recordOffset + maxRecordOutput <= entityIO.getRecordCount()) {
                recordOffset += maxRecordOutput;
                entityIO.seekTo(recordOffset);
                records = entityIO.readNRecords(maxRecordOutput);
            } else {
                cout << "Error: Input is invalid. Re-enter input" << endl;
                cout << "Enter 0 to abort and return to the main menu" << endl << endl;
            }
            clearScreen();
        }
    }

    for (T* record : records) delete record;  // Free memory
    return nullptr;
}

// Function to display a list of items and prompt the user for a selection
template<typename T>
T* selectFromVector(vector<T*>& items, const string& title, const vector<string>& columnNames, void (*printRow)(T& item)) {
    int itemCount = items.size();
    int recordOffset = 0;
    int maxRecordOutput = 10; // Adjust as needed
    string selection;
    int option;
    string range;

    while (true) {
        range = formatSelectionRange(1, itemCount);
        cout << "=== Select " << title << " ===" << endl;

        // Print column headers
        if (!columnNames.empty() && !columnNames[0].empty()) {
            for (const string& columnName : columnNames) {
                cout << columnName << "   ";
            }
            cout << endl;
        }

        // Print rows
        for (int i = 0; i < maxRecordOutput && (recordOffset + i) < itemCount; ++i) {
            cout << to_string(recordOffset + i + 1) << ")";
            if (items[recordOffset + i] != nullptr) {
                printRow(*items[recordOffset + i]);
            } else {
                cout << "Item unavailable";
            }
            cout << endl;
        }

        // Scrolling functionality
        if (maxRecordOutput < itemCount) {
            cout << "*..." << endl;
            if (recordOffset > 0) {
                cout << "p) display previous items" << endl;
            }
            cout << "n) display next items" << endl;
        }

        cout << "ENTER selection " << range << " OR <0> to abort and exit: ";
        getline(cin, selection);

        try {
            option = stoi(selection);
            if (option == 0) return nullptr;

            if (option > 0 && option <= itemCount) {
                T* selectedItem = items[recordOffset + option - 1];
                return selectedItem;
            } else {
                cout << "Error: The option you entered does not exist on the list" << endl << endl;
            }

        } catch (const invalid_argument&) {
            if (selection == "p" && recordOffset - maxRecordOutput >= 0) {
                recordOffset -= maxRecordOutput;
            } else if (selection == "n" && recordOffset + maxRecordOutput < itemCount) {
                recordOffset += maxRecordOutput;
            } else {
                cout << "Error: Input is invalid. Re-enter input" << endl;
                cout << "Enter 0 to abort" << endl << endl;
            }
        }
    }
}
// USE CASES


namespace ScenarioController {
    void createRequestControl() {
        // Get requester type from user
        char reqTSelection;
        do {
            cout << "Is this request coming from a customer or an employee?" << endl;
            cout << "ENTER a selection [c - customer/e - employee]" << endl;
            cout << "OR ENTER <0> to abort and exit to the main menu" << endl;

            cin >> reqTSelection; 
            cin.clear();
            cin.ignore(10000,'\n');
            
            if (reqTSelection == '0') return;
            else if (reqTSelection != 'c' && reqTSelection != 'e') {
                //clearScreen();
                cout << "Error: Input is invalid. Re-enter input" << endl;
                cout << "Enter 0 to abort and return to the main menu" << endl << endl;
            }

            cin.ignore();

        } while (reqTSelection != 'c' && reqTSelection != 'e');

        clearScreen();

        // Get requester from user
        Requester* requester; 
        if(reqTSelection == 'c') requester = selectFromList(requesterIO, "Customer", customerColHeaders, printCustomerRow);
        else {requester = selectFromList(requesterIO, "Employee", employeeColHeaders, printEmployeeRow);}

        if (requester == nullptr) return;
        string requesterName = requester->getName();
        string requesterEmail = requester->getRequesterEmail();
        delete requester;

        // Get date information
        string date;
        do {
            cout << "ENTER the DATE of the request (YYYY-MM-DD) OR ENTER <0> to abort and" << endl;
            cout << "exit to the main menu:";

            getline(cin, date);

            if (date == "0") return;

        } while (date.empty());

        // Get product information
        Product* product = selectFromList(productIO, "Product", noColHeader, printProductRow);
        if (product == nullptr) return;
        string productName = product->getProductName();
        delete product;

        // Get product release information
        ProductRelease* productRelease = selectFromList(productReleaseIO, "Product Release", noColHeader, printProductReleaseRow);
        if (productRelease == nullptr) return;
        int productReleaseID = productRelease->getReleaseID();
        delete productRelease;

        // Get change information
        Change* change = selectFromList(changeIO, "Change Item", changeColHeaders, printChangeRow);
        if (change == nullptr) return;
        string changeDate = change->getDate();
        string changeDesc = change->getDescription();
        delete change;

        // Save request information. TODO: FIX THIS
        Request newRequest(0, requesterEmail.c_str(), productName.c_str(), productReleaseID, Request::Priority::LOW );
        requestIO.appendRecord(newRequest);

        cout << "Request created successfully!" << endl;
    }

    void createRequesterControl() {
        clearScreen();
        
        string requesterEmail = getEmail();
        if (requesterEmail == "0") return;

        clearScreen();

        string phoneInput = getPhone();
        if (phoneInput == "0") return;
        int phone = stol(phoneInput);

        clearScreen();

        string name = getName();
        if (name == "0") return;

        clearScreen();

        string department = getDepartment();
        if (department == "0") return;

        clearScreen();

        Requester newRequester(requesterEmail.c_str(), name.c_str(), phone, department.c_str());
        requesterIO.appendRecord(newRequester);

        cout << "The new requester has been successfully added to the system." << endl;
        cout << "ENTER <0> to go back to the main menu: ";
        string input;
        do {
            getline(cin, input);
        } while (input != "0");
    }

    void createProductControl() {
        clearScreen();
        
        string productName = getProductName();
        if (productName == "0") return;

        clearScreen();

        Product newProduct(productName.c_str());
        productIO.appendRecord(newProduct);

        cout << "The new product has been successfully added to the system." << endl;
        cout << "ENTER <0> to go back to the main menu: ";
        string input;
        do {
            getline(cin, input);
        } while (input != "0");
    }

    void createProductReleaseControl() {
            clearScreen();
            
            string productName = getProductName();
            if (productName == "0") return;

            clearScreen();

            string ReleaseID = getReleaseIDData();
            if (ReleaseID == "0") return;
            int ReleaseIDNumber = stol(ReleaseID);
            
            clearScreen();

            string date = getDateData();
            if (date == "0") return;
            int dateNumber = stol(date);

            clearScreen();

            ProductRelease newProductRelease(productName.c_str(), ReleaseIDNumber, dateNumber);
            productReleaseIO.appendRecord(newProductRelease);

            cout << "The new product release has been successfully added to the system." << endl;
            cout << "ENTER <0> to go back to the main menu: ";
            string input;
            do {
                getline(cin, input);
            } while (input != "0");
        }

    void assessNewChangeControl() {
        // Select a change item
        Change* selectedChange = selectFromList(changeIO, "Change Item", openChangeColHeaders, printOpenChangeRow);
        if (selectedChange == nullptr) return;

        int changeID = selectedChange->getchangeID();
        string productName = selectedChange->getProductName();
        
        // Free memory
        delete selectedChange;

        int statusSelection;
        // Select new status
        cout << "=== Select Status ===" << endl;
        cout << "1) Assessed" << endl;
        cout << "2) Canceled" << endl;
        cin >> statusSelection;

        // Check for exit
        if(statusSelection == 0) return;

        Change::Status status = (statusSelection == 1) ? Change::Status::Assessed : Change::Status::Canceled;

        // Get new description
        string description;
        cout << "ENTER a new description for the change [max 30 characters, leave blank to skip]" << endl;
        cout << "OR <0> to abort and exit to main menu:";
        getline(cin >> ws, description); // `ws` is used to ignore leading whitespace
        // Check for exit
        if(description == "0") return;

        // Select product release
        ProductRelease* selectedProductRelease = selectFromList(productReleaseIO, "Product Release", noColHeader, printProductReleaseRow);
        if (selectedProductRelease == nullptr) return;

        int productReleaseID = selectedProductRelease->getReleaseID();

        // Free memory
        delete selectedProductRelease;

        cout << "=== Updated Change Information ===" << endl;
        cout << "Product: " << productName << endl;
        cout << "Description: " << description << endl;
        cout << "Anticipated Release: " << productReleaseID << endl;
        cout << "Status: " << Change::statusToString(status) << endl;
        cout << "Change ID: " << changeID << endl;
    }


    void updateChangeItemControl() {
        // Fetch initial product list
        vector<Product*> products = productIO.readNRecords(maxRecordOutput);
        Product* selectedProduct = selectFromList(productIO, "Product", noColHeader, printProductRow);
        
        if (!selectedProduct) return;  // Abort if no valid product selected

        string productName = selectedProduct->getProductName();
        delete selectedProduct;  // Free memory

        // Fetch initial change list
        vector<Change*> changes = changeIO.readNRecords(maxRecordOutput);
        Change* selectedChange = selectFromList(changeIO, "Change Item", assessedChangeColHeaders, printAssessedChangeRow);
        
        if (!selectedChange) return;  // Abort if no valid change selected

        int changeID = selectedChange->getchangeID();
        delete selectedChange;  // Free memory

        // Select status
        int statusSelection;
        cout << "=== Select Status ===" << endl;
        cout << "1) In Progress" << endl;
        cout << "2) Done" << endl;
        cout << "3) Canceled" << endl;
        cin >> statusSelection;
        cin.ignore();  // To ignore the newline character left in the buffer

        if (statusSelection == 0) return;  // Abort if exit

        Change::Status status;
        switch (statusSelection) {
            case 1: status = Change::Status::In_Progress; break;
            case 2: status = Change::Status::Done; break;
            case 3: status = Change::Status::Canceled; break;
            default: status = Change::Status::In_Progress; break;
        }

        // Fetch initial product release list
        vector<ProductRelease*> productReleases = productReleaseIO.readNRecords(maxRecordOutput);
        ProductRelease* selectedProductRelease = selectFromList(productReleaseIO, "Product Release", noColHeader, printProductReleaseRow);

        if (!selectedProductRelease) return;  // Abort if no valid product release selected

        int productReleaseID = selectedProductRelease->getReleaseID();
        delete selectedProductRelease;  // Free memory

        // TODO: GET DESCRIPTION

        // Output updated change information
        cout << "=== Updated Change Information ===" << endl;
        cout << "Product: " << productName << endl;
        cout << "Description: " << "THIS IS A PLACEHOLDER" << endl;
        cout << "Anticipated Release: " << productReleaseID << endl;
        cout << "Status: " << Change::statusToString(status) << endl;
        cout << "Change ID: " << changeID << endl;
    }


    void inquireChangeItemControl() {
        clearScreen();

        // Step 1: Enter Inquire Menu
        cout << "=== Inquire Menu ===" << endl;

        // Fetch product records
        vector<Product*> products = productIO.readNRecords(productIO.getRecordCount());
        if (products.empty()) {
            cout << "No products available for inquiry." << endl;
            return;
        }

        Product* selectedProduct = selectFromList(productIO, "Product", noColHeader, printProductRow);

        if (!selectedProduct) return;  // Abort if no valid product selected
        string productName = selectedProduct->getProductName();
        delete selectedProduct;

        clearScreen();

        // Step 2: Select a change item to inquire
        vector<Change*> changes = changeIO.readNRecords(changeIO.getRecordCount());
        Change* selectedChange = selectFromList(changeIO, "Change Item", assessedChangeColHeaders, printAssessedChangeRow);

        if (!selectedChange) return;  // Abort if no valid change selected

        // Output change item details
        cout << "Change Item Report:" << endl;
        cout << "Product name: " << productName << endl;
        cout << "ChangeID: " << selectedChange->getchangeID() << endl;
        cout << "First Reported: " << selectedChange->getDate() << endl;
        cout << "Status: " << Change::statusToString(selectedChange->getStatus()) << endl;
        cout << "Anticipated release: " << selectedChange->getAnticipatedReleaseID() << endl;
        cout << "Description: " << selectedChange->getDescription() << endl << endl;

        delete selectedChange;

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


    // Updated printScenario1Control using getProductReleaseRecords and getProductRecords
    void printScenario1Control() {
        // Fetch product records
        vector<Product*> products = productIO.readNRecords(productIO.getRecordCount());
        if (products.empty()) {
            cout << "Error fetching product records. Aborting." << endl;
            return;
        }

        Product* selectedProduct = selectFromList(productIO, "Product", noColHeader, printProductRow);
        if (!selectedProduct) {
            // Clean up products if no valid product selected
            for (Product* product : products) {
                delete product;
            }
            return;
        }

        vector<ProductRelease*> productReleases = productReleaseIO.readNRecords(productReleaseIO.getRecordCount());

        // Filter product releases to keep only those matching the selected product's name
        string selectedProductName = selectedProduct->getProductName();
        for (auto it = productReleases.begin(); it != productReleases.end(); ) {
            if ((*it)->getProductName() != selectedProductName) {
                // Remove product release if it does not match the selected product
                delete *it;  // Delete the ProductRelease object before erasing the pointer
                it = productReleases.erase(it);
            } else {
                ++it;
            }
        }

        ProductRelease* selectedProductRelease = selectFromVector(productReleases, "ProductRelease", noColHeader, printProductReleaseRow);
        if (selectedProductRelease) {
            PrintController::initPrintController();
            PrintController::printProduct(*selectedProductRelease); 
            PrintController::exitPrint();

            cout << "Report for \"" << selectedProductName << "\" version: \"" << selectedProductRelease->getReleaseID() << "\" has been successfully printed." << endl;
        } else {
            cout << "No valid product release selected. Aborting." << endl;
        }

        // Clean up remaining product releases
        for (ProductRelease* release : productReleases) {
            delete release;
        }
        productReleases.clear();

        // Clean up products
        for (Product* product : products) {
            delete product;
        }

        int continueOption = -1;
        while (continueOption != 0 && continueOption != 1) {
            cout << "Enter <1> to print another report." << endl;
            cout << "Enter <0> to go back to the main menu." << endl;
            cout << "ENTER Selection: ";

            cin >> continueOption;
            cin.ignore(10000,'\n');

            if (continueOption == 0) {
                clearScreen();
                return; // Return to main menu
            } else if (continueOption == 1) {
                clearScreen();
                printScenario1Control(); // Recursive call (consider using a loop for production code)
                return;
            } else {
                clearScreen();
                cout << "Error: Invalid input. Please enter 0 or 1." << endl;
            }
        }

        clearScreen();
}

        // Updated printScenario2Control using getProductRecords
        void printScenario2Control() {
            // Fetch product records
            vector<Product*> products = productIO.readNRecords(productIO.getRecordCount());
            if (products.empty()) {
                cout << "Error fetching product records. Aborting." << endl;
                return;
            }

            Product* selectedProduct = selectFromList(productIO, "Product", noColHeader, printProductRow);
            if (!selectedProduct) {
                for (Product* product : products) {
                    delete product;
                }
                return;  // Abort if no valid product selected
            }

            vector<Change*> changes = changeIO.readNRecords(changeIO.getRecordCount());

            // Filter changes to keep only those matching the selectedProduct's name and status "done"
            string selectedProductName = selectedProduct->getProductName();
            for (auto it = changes.begin(); it != changes.end(); ) {
                if ((*it)->getProductName() != selectedProductName || Change::statusToString((*it)->getStatus()) != "done") {
                    // Remove change if it does not match the selected product name or status
                    delete *it;  // Delete the Change object before erasing the pointer
                    it = changes.erase(it);
                } else {
                    // Move to the next change if it matches
                    ++it;
                }
            }

            Change* selectedChange = selectFromVector(changes, "Change", noColHeader, printChangeRow);
            if (selectedChange) {
                PrintController::initPrintController();
                PrintController::printCompletedChangeItems(*selectedChange);
                PrintController::exitPrint();

                cout << "The report for the completed change item has been successfully printed." << endl;
            } else {
                cout << "No valid change item selected. Aborting." << endl;
            }

            // Clean up remaining changes
            for (Change* change : changes) {
                delete change;
            }
            changes.clear();

            for (Product* product : products) {
                delete product;
            }

            int continueOption = -1;
            while (continueOption != 0 && continueOption != 1) {
                cout << "Enter <1> to print another report." << endl;
                cout << "Enter <0> to go back to the main menu." << endl;
                cout << "ENTER Selection: ";

                cin >> continueOption;
                cin.ignore(10000,'\n');
        // Clear input buffer

                if (continueOption == 0) {
                    clearScreen();
                    return; // Return to main menu
                } else if (continueOption == 1) {
                    clearScreen();
                    printScenario2Control(); // Call the function again for another report
                    return;
                } else {
                    clearScreen();
                    cout << "Error: Invalid input. Please enter 0 or 1." << endl;
                }
            }

            clearScreen();
        }

    }
