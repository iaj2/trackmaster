#include "ScenarioController.h"
#include "PrintController.h"
#include "Backup.h"
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
using namespace Backup;

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
        getline(cin, input);

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

// USE CASES


namespace ScenarioController {

    void backupControl() {
        cin.ignore(10000,'\n');
        clearScreen();

        char backupSelection;
        do {
            cout << "Your backed-up files will be stored in your external hard drive, please" << endl;
            cout << "Make sure the drive is connected." << endl;
            cout << "The back-up process can take up to 10 minutes. " << endl;
            cout << "Do you wish to proceed with the back-up procedure ? (Y/N)" << endl;
            cout << "ENTER Selection: " << endl;

            cin >> backupSelection;
            cin.ignore(10000,'\n');
            
            if (backupSelection == 'N') return;

            else if (backupSelection != 'Y' && backupSelection != 'N') {
                cout << "Error: Input is invalid. Re-enter input" << endl;
                cout << "Enter 0 to abort and return to the main menu" << endl << endl;
            }
            cin.ignore();

        } while (backupSelection != 'Y' && backupSelection != 'N');

        clearScreen();

        if (backupSelection == 'Y') {
            std::vector<std::string> fileNames = {"change.dat", "product.dat", "productRelease.dat", "request.dat", "requester.dat"};

            try {
                initBackup(fileNames);
                backup(fileNames);
            }
            catch (const std::runtime_error& e) {
                std::cout << "Exception caught: " << e.what() << std::endl;
            }
        }

        char backupReturn;
        do {
            cout << "The back-up procedure has completed successfully" << endl;
            cout << "Enter <0> to go back to the main menu." << endl;
            cout << "ENTER selection: " << endl;
            
            cin >> backupReturn;
            cin.ignore(10000,'\n');

        } while (backupReturn != '0');

        return;
    }

    void createRequestControl() {
        // Get requester type from user
        cin.ignore(10000,'\n');
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
        cin.ignore(10000,'\n');
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
        cin.ignore(10000,'\n');
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
        cin.ignore(10000,'\n');
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
        string description;
        do {
        cout << "ENTER a new description for the change (leave blank to skip) OR <0> to abort and exit to main menu [max 30 characters]:" << endl;
        getline(cin, description);

        if (description.length() > Change::MAX_DESCRIPTION_LENGTH) {
            clearScreenAndShowError("Input is too long.");
        } 
        } while (description.length() > Change::MAX_DESCRIPTION_LENGTH);

        // Output updated change information
        cout << "=== Updated Change Information ===" << endl;
        cout << "Product: " << productName << endl;
        cout << "Description: " << description << endl;
        cout << "Anticipated Release: " << productReleaseID << endl;
        cout << "Status: " << Change::statusToString(status) << endl;
        cout << "Change ID: " << changeID << endl;
    }


    void inquireChangeItemControl() {
        cin.ignore(10000,'\n');
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

        if (!selectedProduct) return;  // Abort if no valid product selected

        // Fetch product release records
        vector<ProductRelease*> releases = productReleaseIO.readNRecords(4);  // Adjust count if needed
        if (releases.empty()) {
            cout << "Error fetching product release records. Aborting." << endl;
            delete selectedProduct;
            return;
        }

        ProductRelease* selectedRelease = selectFromList(productReleaseIO, "Product Release", noColHeader, printProductReleaseRow);

        if (!selectedRelease) {
            delete selectedProduct;
            return;  // Abort if no valid product release selected
        }

        // Call PrintController method with selected product and release
        PrintController::printProduct(*selectedRelease);

        delete selectedProduct;
        delete selectedRelease;
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

        if (!selectedProduct) return;  // Abort if no valid product selected

        delete selectedProduct;
    }
}

