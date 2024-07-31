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
#include "Backup.h"
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <functional>

using namespace std;
using namespace UI;
using namespace Backup;

// Some global constants
const int maxRecordOutput = 5; 

enum scenarioState {Create, Assess, Update, P2Control, Blank};

// Some helper functions

string formatSelectionRange(int start, int end) {
    return "[" + to_string(start) + "-" + to_string(end) + "]";
}

// check that a string can be converted to integer, and is within a range
bool isValidIntegerInRange(const string& str, int min, int max) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    int value = stoi(str);
    return value >= min && value <= max;
}

void clearScreenAndShowError(const string& errorMessage) {
    clearScreen();
    cout << "Error: " << errorMessage << " Please re-enter." << endl << endl;
}

// Function to check if a string is only whitespace
bool isBlank(const string& str) {
    return all_of(str.begin(), str.end(), [](unsigned char c) { return isspace(c); });
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
        } else if (input.empty() || isBlank(input)) clearScreenAndShowError("Input is invalid");
        else {
            return input;
        }
    } while (true);
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
    string dep;
    do {
        cout << "ENTER the DEPARTMENT NAME of the requester (Length: max 12)" << endl;
        cout << "***Must be left blank if requester is not an employee of the company***" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";

        getline(cin, dep);

        if (dep == "0") {
            // Abort and exit to the main menu
            return "";
        }

        if (dep.length() > Requester::MAX_DEPARTMENT_LENGTH) {
            clearScreenAndShowError("Input too long.");
        } else if (isBlank(dep) || dep.length() < 1) {
            dep = "";
            return dep;
        } else {
            // Valid input
            return dep;
        }

    } while (true);

    return dep; // This will never be reached, but is kept for completeness
}


string getProductName() {
    return getInput("ENTER the PRODUCT NAME (Length: max 10)", Product::MAX_PRODUCT_NAME_LENGTH);
}


string getReleaseIDData() {
    string releaseIDInput;
    do {
        cout << "ENTER the release ID of the Product Release (integers, Length: at most 4)" << endl;
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

// count customer type requesters
int countCustomers() {
    int count = 0;
    requesterIO.seekToStart();

    while (true) {
        Requester* requester = requesterIO.readRecord();

        // if department is empty, must be a customer
        if (requester != nullptr && isBlank(requester->getDepartment())) {
            count++;
        } else if (requester == nullptr) {
            break; // If readRecord returns nullptr, stop reading
        }
    }
    return count;
}

// count employee type requesters
int countEmployees() {
    int count = 0;
    requesterIO.seekToStart();

    while (true) {
        Requester* requester = requesterIO.readRecord();

        // if department is not empty, must be an employee
        if (requester != nullptr && !isBlank(requester->getDepartment())) {
            count++;
        } else if (requester == nullptr) {
            break; // If readRecord returns nullptr, stop reading
        }
    }
    return count;
}

// count product releases for a product
int countProductReleases(const string& productName) {
    int count = 0;
    productReleaseIO.seekToStart();

    while (true) {
        ProductRelease* pr = productReleaseIO.readRecord();

        if (pr != nullptr && pr->getProductName() == productName) {
            count++;
        } else if (pr == nullptr) {
            break; // If readRecord returns nullptr, stop reading
        }
    }
    return count;
}

// count change items by product name and status
int countChangeItems(const string& productName, const string& status = "") {
    int count = 0;
    changeIO.seekToStart();

    while (true) {
        Change* change = changeIO.readRecord();

        if (change != nullptr) {
            bool matchesProductName = (productName != "" && productName == change->getProductName()) || productName == "";
            bool matchesStatus = (status != "" && status == Change::statusToString(change->getStatus())) || status == "";

            if (matchesProductName && matchesStatus) {
                count++;
            }
        } else {
            break; // If readRecord returns nullptr, stop reading
        }
    }
    return count;
}

int getChangeIndex(const Change selectedchange) {
    int count = 0;
    changeIO.seekToStart();

    while (true) {
        Change* change = changeIO.readRecord();

        if (change != nullptr) {
           if (change->getchangeID() == selectedchange.getchangeID()) {
            return count;
           }
        } else {
            break; // Record assumed exists 
        }
        count++;
    }
    return count;
}

int dateToInt(const char* date) {
    std::string dateStr(date);
    // Remove the dashes
    dateStr.erase(remove(dateStr.begin(), dateStr.end(), '-'), dateStr.end());
    // Convert the string to an integer
    int dateInt = std::stoi(dateStr);
    return dateInt;
}

void printListOptions(int recordIndex, int recordCount, string range) {
    if (maxRecordOutput < recordCount) cout << "*..." << endl;
    if(recordIndex + maxRecordOutput < recordCount) cout << "n) display next items" << endl;
    if (recordIndex+1 > maxRecordOutput) cout << "p) display previous items" << endl;
    cout << "ENTER selection " << range << " OR <0> to abort and exit to the main menu: ";
}

bool userSelectedNext(string selection, int recordIndex) {
    return (selection == "p" && recordIndex - maxRecordOutput >= 0);
}

bool userSelectedPrev(string selection, int recordIndex, int recordCount) {
    return (selection == "n" && recordIndex + maxRecordOutput < recordCount);
}


// gets n customer type requesters
vector<Requester*> fetchNCustomers(int n, int recordIndex) {
    vector<Requester*> customers;
    int count = 0;

    requesterIO.seekTo(recordIndex);
    
    while (count < n) {
        Requester* requester = requesterIO.readRecord();

        if(requester == nullptr) {
            break; // If readRecord returns nullptr, stop reading
        }

        // if department is empty, must be a customer
        if (isBlank(requester->getDepartment())) {
            customers.push_back(requester);
            count++;
        } else {
            delete requester;
        }
    }
    return customers;
}

// gets n employee type requesters
vector<Requester*> fetchNEmployees(int n, int recordIndex) {
    vector<Requester*> employees;
    int count = 0;

    requesterIO.seekTo(recordIndex);
    
    while (count < n) {
        Requester* requester = requesterIO.readRecord();

        if (requester == nullptr) {
            break; // If readRecord returns nullptr, stop reading
        }

        // if department is not empty, must be an employee
        if (!isBlank(requester->getDepartment())) {
            employees.push_back(requester);
            count++;
        } else {
            delete requester; 
        }
    }

    return employees;
}

// gets n product releases by product name
vector<ProductRelease*> fetchNProductReleases(int n, int recordIndex, string productName) {
    vector<ProductRelease*> prs;
    int count = 0;

    productReleaseIO.seekTo(recordIndex);

    while (count < n) {
        ProductRelease* pr = productReleaseIO.readRecord();

        if (pr == nullptr) {
            // Handle the case where no more records are available
            break;
        }
        if (pr->getProductName() == productName) {
            prs.push_back(pr);
            count++;
        } else {
            // If the product name does not match, continue to the next record
            delete pr; // Clean up to prevent memory leaks
        }
    }

    return prs;
}

// gets n change items by product name and status. if status left blank, get change item of any status
// if product name is blank, do not filter by product name
vector<Change*> fetchNChangeItems(int n, int recordIndex, string productName, string status = "") {
    vector<Change*> changeItems;
    int count = 0;

    changeIO.seekTo(recordIndex);

    while (count < n) {
        Change* change = changeIO.readRecord();

        if(change == nullptr) break; // stop reading on null

        if ((productName != "" && productName == change->getProductName()) || productName == "") {
            if ((status != "" && status == Change::statusToString(change->getStatus())) || status == "") {
            changeItems.push_back(change);
            count++;
            }
        } else {
            delete change;
        }
       
    }

    return changeItems;
}

int selectProductReleaseID(string productName, scenarioState state) {
    // start at beginning of records
    int recordIndex = 0;

    // get first set of records
    vector<ProductRelease*> productRels = fetchNProductReleases(maxRecordOutput, recordIndex, productName);

    // Get record count
    int recordCount = countProductReleases(productName);

    //
    string selection;
    int option;
    ProductRelease* selectedProductRel;

    while (true) {
        cout << "=== Select Product Release ===" << endl;

        // print rows
        for(int i=0; i < productRels.size(); i++) {
            if (productRels[i] != nullptr) {
                cout << to_string(recordIndex + i + 1) << ") " << to_string(productRels[i]->getReleaseID()) << endl;
            }
            else {
                cout << "Record unavailable" << endl;
            }
        }
        
        // Option to skip
        if ((state== Assess || state== Update) && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(recordCount+1) << ") Skip" << endl;
        }

        // print options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, recordCount));

        // get user input
        getline(cin, selection);

        if (userSelectedNext(selection, recordIndex)) {
                recordIndex -= maxRecordOutput;
                productRels = fetchNProductReleases(maxRecordOutput, recordIndex, productName);
        } else if (userSelectedPrev(selection, recordIndex, recordCount)) {
            recordIndex += maxRecordOutput;
            productRels = fetchNProductReleases(maxRecordOutput, recordIndex, productName);
        } else {
            if(isValidIntegerInRange(selection, 0 , recordCount)) {
                option = stoi(selection);
                if (option == 0) return 0;
                if (state==Assess && option  == recordCount + 1) {
                    return -1; // signals skip
                } else {
                    selectedProductRel = productRels[option - recordIndex - 1];
                    // for (ProductRelease* p : productRels) delete p;  // Free memory
                    return selectedProductRel->getReleaseID();
                }
            } else {
                clearScreenAndShowError("Option you entered does not exist on the list.");
                cout << endl;
            }
        }
    }
}

// Covers steps 2-6 of the "Create Requester" use case. Returns a new requester based off user input
Requester* createNewRequester() {
        clearScreen();
        string requesterEmail = getEmail();
        if (requesterEmail == "0") return nullptr;

        clearScreen();

        string phoneInput = getPhone();
        if (phoneInput == "0") return nullptr;
        int phone = stol(phoneInput);

        clearScreen();

        string name = getName();
        if (name == "0") return nullptr;

        clearScreen();

        string department = getDepartment();
        if (department == "0") return nullptr;

        clearScreen();

        Requester* newRequester = new Requester(requesterEmail.c_str(), name.c_str(), phone, department.c_str());

        return newRequester;
}

// Covers step 3 of the "Create Product" use case. Returns a new product based off user input
Product* createNewProduct() {
    clearScreen();
    
    string productName = getProductName();
    if (productName == "0") return nullptr;

    clearScreen();

    Product* newProduct = new Product(productName.c_str());
    return newProduct;
}

// TODO: FIX THIS
ProductRelease* createNewProductRelease() {
    clearScreen();
            
    string productName = getProductName();
    if (productName == "0") return nullptr;

    clearScreen();

    string ReleaseID = getReleaseIDData();
    if (ReleaseID == "0") return nullptr;
    int ReleaseIDNumber = stol(ReleaseID);
    
    clearScreen();

    string date = getDateData();
    if (date == "0") return nullptr;
    int dateNumber = stol(date);

    clearScreen();

    ProductRelease* newProductRelease = new ProductRelease(productName.c_str(), ReleaseIDNumber, dateNumber);

    return newProductRelease;
}

// Step 10-11 of the "Create Request" use case. Returns a new change based off user input
Change* createNewChangeItem(string productName) {
    clearScreen();

    string changeDescription = getInput("ENTER a description for a new change (30 char. max)", Change::MAX_DESCRIPTION_LENGTH);
    if (changeDescription == "0") return nullptr;

    clearScreen();
    cout << "Select a product product release that the change is anticipated to completed for" << endl;
    int productRelease = selectProductReleaseID(productName, Blank);

    // TODO: DATE
    Change* newChangeItem = new Change(Change::Status::Open, productName.c_str(), productRelease, 
                                        changeDescription.c_str(), 1231);

    return newChangeItem;
}

// Select product from list
Product* selectProduct(scenarioState state) {
    clearScreen();
    // start at beginning of records
    int recordIndex = 0;
    
    // get first set of records
    productIO.seekToStart();
    vector<Product*> products = productIO.readNRecords(maxRecordOutput);

    // get record count
    int recordCount = productIO.getRecordCount();

    // 
    string selection;
    int option;
    Product* selectedProduct;
    // get what last option is based on state
    int maxSelection = recordCount;
    if(state == scenarioState::Create) {
        maxSelection += 1;
    }

    while (true) {
        cout << "=== Select Product ===" << endl;

        // print rows
        for(int i=0; i < products.size(); i++) {
            if (products[i] != nullptr) {
                cout << to_string(recordIndex+i+1) << ") " << products[i]->getProductName() << endl;
            }
            else {
                cout << "Record unavailable" << endl;
            }
        }
        // Option to create a new item
        if (state== Create && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(recordCount + 1) << ") New Product" << endl; 
        }

        // print options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, maxSelection));

        // get user input
        getline(cin, selection);
        
        if (userSelectedNext(selection, recordIndex)) {
                recordIndex -= maxRecordOutput;
                productIO.seekTo(recordIndex);
                products = productIO.readNRecords(maxRecordOutput);
                clearScreen();
        } else if (userSelectedPrev(selection, recordIndex, recordCount)) {
            recordIndex += maxRecordOutput;
            productIO.seekTo(recordIndex);
            products = productIO.readNRecords(maxRecordOutput);
            clearScreen();
        } else {
                if(isValidIntegerInRange(selection, 0, maxSelection)){
                    option = stoi(selection);
                    if (option == 0) return nullptr;
                    if (state== Create && option  == maxSelection) {
                        // Handle creating a new item
                        selectedProduct = createNewProduct();
                        productIO.appendRecord(*selectedProduct);
                        cout << "New Product created!" << endl << endl;
                        return selectedProduct; // Return new item
                    } else {
                        selectedProduct = products[option - recordIndex - 1];
                        // for (Product* product : products) delete product;  // Free memory
                        return selectedProduct;
                    } 
                    
                } else {
                    clearScreenAndShowError("Invalid input.");
                }
                
            }
    }
}


Requester* selectRequester(scenarioState state, string type) {
    clearScreen();
    // start of beginning of records
    int recordIndex = 0;

    // get first set of records
    vector<Requester*> requesters;
    int recordCount;
    
    if(type == "c"){
        requesters = fetchNCustomers(maxRecordOutput, recordIndex);
        recordCount = countCustomers();
    } 
    else {
        requesters = fetchNEmployees(maxRecordOutput, recordIndex);
        recordCount = countEmployees();
    }

    //
    string selection;
    int option;
    Requester* selectedRequester;
    // get what last option is based on state
    int maxSelection = recordCount;
    if(state == scenarioState::Create) {
        maxSelection += 1;
    }

    while (true) {
        cout << "=== Select ";
        if (type=="c") cout << "Customer";
        else cout << "Employee";
        cout << " ===" << endl;

        // print column headers
        cout << "   Name       " << "    Email";
        if(type=="e") {
            cout << "                         Department";
        }
        cout << endl;

        // print rows
        for(int i=0; i < requesters.size(); i++) {
            if (requesters[i] != nullptr) {
                cout << to_string(recordIndex+i+1) << ") ";
                cout << requesters[i]->getName(); 

                const char * tempName = requesters[i]->getName();
                for(int i = 0; i < (Requester::MAX_NAME_LENGTH - strlen(tempName)); i++ ){
                    cout << " "; 
                }
                
                cout << requesters[i]->getRequesterEmail();

                const char * tempEmail = requesters[i]->getRequesterEmail();
                for(int i = 0; i < (Requester::MAX_EMAIL_LENGTH - strlen(tempEmail)); i++ ){
                    cout << " "; 
                }
                // if employee, display department
                if(type=="e") cout << requesters[i]->getDepartment();
                cout << endl;
            } 
            else {
                cout << "Record unavailable" << endl;
            }
        }
        // Option to create a new item
        if (state == Create  && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(recordCount + 1) << ") New ";
            if (type == "c") cout << "Customer" << endl;
            else cout << "Employee" << endl;
        }

        // print options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, maxSelection));

        // get user input
        getline(cin, selection);

        // get next set of items
        if (userSelectedNext(selection, recordIndex)) {
                recordIndex -= maxRecordOutput;
                if(type == "c") {requesters = fetchNCustomers(maxRecordOutput, recordIndex); } 
                else { requesters = fetchNEmployees(maxRecordOutput, recordIndex);}
                clearScreen();
                
        // get prev set of items
        } else if (userSelectedPrev(selection, recordIndex, recordCount)) {
            recordIndex += maxRecordOutput;
            if(type == "c") {requesters = fetchNCustomers(maxRecordOutput, recordIndex); }
            else {requesters = fetchNEmployees(maxRecordOutput, recordIndex);};
            clearScreen();
        // user selection
        } else {
            if(isValidIntegerInRange(selection, 0, maxSelection)) {
                option = stoi(selection);
                if (option == 0) return nullptr;
                if (state==scenarioState::Create && option  == maxSelection) {
                    // Handle creating a new item
                    selectedRequester = createNewRequester();
                    requesterIO.appendRecord(*selectedRequester);
                    cout << "New Requester created!" << endl << endl;
                    return selectedRequester; // Return new item
                } else {
                    selectedRequester = requesters[option - recordIndex - 1];
                    // for (Requester* req : requesters) delete req;  // Free memory
                    return selectedRequester;
                }
            } else{
                clearScreenAndShowError("Invalid input.");
            }
                
        }
    }
}

// Select change item from list
Change* selectChange(string productName, scenarioState state) {
    clearScreen();
    // start at beginning of records
    int recordIndex = 0;
    changeIO.seekToStart();

    // filter based on status
    string statusFilter;
    if (state == Assess) statusFilter = "Open";
    else if (state == Update) statusFilter = "Assessed";
    else if (state == P2Control) statusFilter = "Done";
    else statusFilter = "";

    // get first set of records
    vector<Change*> changes = fetchNChangeItems(maxRecordOutput, recordIndex, productName, statusFilter);

    // Get record count
    int recordCount = countChangeItems(productName, statusFilter);

    //
    string selection;
    int option;
    Change* selectedChange;
    // get what last option is based on state
    int maxSelection = recordCount;
    if(state == scenarioState::Create) {
        maxSelection += 1;
    }

    while (true) {
        cout << "=== Select Change Item ===" << endl;

        // print headers
        if(state == scenarioState::Create) {
            cout << "Description            " << "ChandeID" << endl;
        }
        else if(state == scenarioState::Assess) {
            cout << "Product        " << "Description       " << "Status" << endl; 
        }
        else {
            cout << "Description        " << "Status        " << "ChangeID" << endl;
        }


        // print rows
        for(int i=0; i < changes.size(); i++) {
            if (changes[i] != nullptr){
                cout << to_string(i+recordIndex+1) << ") ";
                // create display
                if(state == Create ){
                    cout << changes[i]->getDescription() << "   " << to_string(changes[i]->getchangeID());
                } 
                // assess display
                else if (state == Assess){
                    cout << changes[i]->getProductName() << "   " << changes[i]->getDescription();
                    cout << "   " << Change::statusToString(changes[i]->getStatus());
                }      
                // update, print display
                else {
                    
                    cout << changes[i]->getDescription() << "   ";
                    cout << Change::statusToString(changes[i]->getStatus()) << "   " << to_string(changes[i]->getchangeID());
                }
            }
            else {
                cout << "Record unavailable";
            }
            cout << endl;
        }

        // Option to create a new item
        if (state==Create  && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(recordCount + 1) << ") New Change Item" << endl; 
        }

        // print options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, maxSelection));

        // get user input
        getline(cin, selection);

        if (userSelectedNext(selection, recordIndex)) {
            recordIndex -= maxRecordOutput;
            changes = fetchNChangeItems(maxRecordOutput, recordIndex, productName, statusFilter);
            clearScreen();
        } else if (userSelectedPrev(selection, recordIndex, recordCount)) {
            recordIndex += maxRecordOutput;
            changes = fetchNChangeItems(maxRecordOutput, recordIndex, productName, statusFilter);
            clearScreen();
        } else {
            if (isValidIntegerInRange(selection, 0, maxSelection)) {
                option = stoi(selection);

                if (option == 0) return nullptr;

                if (state==Create  && option == maxSelection) {
                    // Handle creating a new item
                    selectedChange = createNewChangeItem(productName);
                    changeIO.appendRecord(*selectedChange);
                    cout << "New Change Item created!" << endl << endl;
                    return selectedChange; // Return new item
                } else {
                    selectedChange = changes[option - recordIndex - 1];
                    // for (Change* change : changes) delete change;  // Free memory
                    return selectedChange;
                }
            } else {
                clearScreenAndShowError("Invalid input.");
            }
        }
    }
}

int selectPriority() {
    clearScreen();
    string selection;
    do {
        cout << "=== Select a Priority ===" << endl;
        cout << "1) Lowest" << endl;
        cout << "2) Low" << endl;
        cout << "3) Medium" << endl;
        cout << "4) High" << endl;
        cout << "5) Highest" << endl;
        cout << "ENTER selection [1-5] OR <0> to abort and exit to the main menu: ";

        getline(cin, selection);

        if (selection == "0") {
            return -1;
        }

        if (isValidIntegerInRange(selection, 1, 5)) {
            return stoi(selection);
        } else {
            clearScreenAndShowError("Invalid Input.");
        }
    } while (true);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 
// Scenario Control name space: includes all of the exported functions for scenario control 
// 
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace ScenarioController {
    // -------------------------------------------------------------------------------------------------------------------
    // backupControl: Back up scenario controller 
    // -------------------------------------------------------------------------------------------------------------------
    void backupControl() {
        // clear the input buffer 
        cin.ignore(10000,'\n');
        clearScreen();

        // loop to get user input selection in the backup menu 
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
                clearScreen();
                cout << "Error: Input is invalid. Re-enter input" << endl << endl;
            }
            cin.ignore();

        } while (backupSelection != 'Y' && backupSelection != 'N');

        clearScreen();

        // assess the input and innitiate the backup 
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

        // loop to get user input selection in the backup menu 
        char backupReturn;
        do {
            cout << "The back-up procedure has completed successfully" << endl;
            cout << "Enter <0> to go back to the main menu." << endl;
            cout << "ENTER selection: " << endl;
            
            cin >> backupReturn;
            cin.ignore(10000,'\n');

            if(backupReturn != '0') {
                clearScreen();
                cout << "Invalid Input." << endl << endl;
            }

        } while (backupReturn != '0');
    }
    // -------------------------------------------------------------------------------------------------------------------
    // createRequestControl: creating a request scenario controller 
    // -------------------------------------------------------------------------------------------------------------------
    void createRequestControl() {
        // clear the input buffer
        cin.ignore(); 
        
        // loop to get user input selection 
        string reqTSelection;
        do {
            cout << "Is this request coming from a customer or an employee?" << endl;
            cout << "ENTER a selection [c - customer/e - employee]" << endl;
            cout << "OR ENTER <0> to abort and exit to the main menu" << endl;

            // get user input
            getline(cin, reqTSelection); 
            
            if (reqTSelection == "0") return;
            else if (reqTSelection != "c" && reqTSelection != "e") {
                clearScreenAndShowError("Invalid Input");
            }

        } while (reqTSelection != "c" && reqTSelection != "e");

        clearScreen();

        // Get requester from user
        Requester* requester = selectRequester(scenarioState::Create, reqTSelection);

        if (requester == nullptr) return;

        string requesterName = requester->getName();
        string requesterEmail = requester->getRequesterEmail();

        delete requester;

        // Loop to Get date information
        string date;
        do {
            cout << "ENTER the DATE of the request (YYYY-MM-DD) OR ENTER <0> to abort and" << endl;
            cout << "exit to the main menu:";

            getline(cin, date);

            if (date == "0") return;

            clearScreen();

        } while (date.empty());

        // Get product information
        Product* product = selectProduct(scenarioState::Create);
        if (product == nullptr) return;
        string productName = product->getProductName();
        delete product;

        // get product release id
        int productReleaseID = selectProductReleaseID(productName, scenarioState::Create);
        if (productReleaseID == 0) return;

        // get priority
        int selectedPriority = selectPriority();
        Request::Priority priority;
        string priorityStr;
        switch (selectedPriority)
        {
            case 1:
                priority = Request::Priority::LOW; 
                priorityStr  = "1 - Low"; break;
            case 2:
                priority = Request::Priority::MEDIUM;
                priorityStr = "2 - Medium"; break;
            case 3:
                priority = Request::Priority::HIGH;
                priorityStr = "3 - High"; break;
            case 4:
                priority = Request::Priority::VERY_HIGH;
                priorityStr = "4 - Very High"; break;
            case 5:
                priority = Request::Priority::CRITICAL;
                priorityStr = "5 - Critical"; break;
            default: // medium priority by default
                priority = Request::Priority::MEDIUM; 
                priorityStr = "2 - Medium";
                break;
        }

        // get change information
        Change* change = selectChange(productName, Create);
        if (change == nullptr) return;
        int changeID = change->getchangeID();
        string changeDate = change->getDate();
        string changeDesc = change->getDescription();
        delete change;

        // confirm new request
        string confirmSel;
        string reqType = (reqTSelection == "c") ? "customer" : "employee";

        // loop to present the change to the screen  
        do {
            cout << "=== New Request Information ===" << endl;
            cout << "Requester (" << reqType << "): " << requesterName << " - " << requesterEmail << endl;
            cout << "Product: " << productName << endl;
            cout << "Release: " << to_string(productReleaseID) << endl;
            cout << "Priority: " << priorityStr << endl;
            cout << "Change ID: " << to_string(changeID);
            cout << "ENTER <1> to confirm OR <0> to abort and exit to main menu: ";

            getline(cin, confirmSel);

            if (confirmSel == "0") return; // abort
            else if (confirmSel != "1") clearScreenAndShowError("Invalid Input");
            cout << endl;
        } while (confirmSel != "1");

        // record request
        Request newRequest(changeID, requesterEmail.c_str(), productName.c_str(), productReleaseID, priority);
        requestIO.appendRecord(newRequest);
        
    }

// -------------------------------------------------------------------------------------------------------------------
// createRequesterControl: scenario controller for creating a requester
// -------------------------------------------------------------------------------------------------------------------
    void createRequesterControl() {
        // clear the input buffer 
        cin.ignore();
        // call create requester function 
        Requester* newRequester = createNewRequester();
        
        // make it is not null 
        if (newRequester == nullptr) return;

        // append to memory 
        requesterIO.appendRecord(*newRequester);
        delete newRequester; // free memory

        // loop to get user input selection upon exit 
        string input;
        do {
            cout << "The new requester has been successfully added to the system." << endl;
            cout << "ENTER <0> to go back to the main menu: ";
        
            getline(cin, input);

            if (input == "0") return;

            clearScreenAndShowError("Invalid Input");
            cout << endl;
        } while (true);
    }

    // -------------------------------------------------------------------------------------------------------------------
    // createProductControl: scenario controller for creating a new product 
    // -------------------------------------------------------------------------------------------------------------------
    void createProductControl() {
        cin.ignore(); 
        // call create new product helper function 
        Product* newProduct = createNewProduct();
        
        // make sure the return is valid 
        if (newProduct == nullptr) return;

        // append to memory 
        productIO.appendRecord(*newProduct);
        delete newProduct; // free memory

        // loop to get user input selection upon exit 
        string input;
        do {
            cout << "The new product has been successfully added to the system." << endl;
            cout << "ENTER <0> to go back to the main menu: ";
            getline(cin, input);

            if(input == "0") return;

            clearScreenAndShowError("Invalid Input.");
        } while (true);
    }

    // -------------------------------------------------------------------------------------------------------------------
    // createProductReleaseControl: Scenario controller for creating a new product release 
    // -------------------------------------------------------------------------------------------------------------------
    void createProductReleaseControl() {
        // clear the input buffer 
        cin.ignore();
        
        // call create new product release helper function 
        ProductRelease* newProductRelease = createNewProductRelease();
        
        // check to make sure the return is valid 
        if (newProductRelease == nullptr) return;
        
        // append the record 
        productReleaseIO.appendRecord(*newProductRelease);

        cout << "The new product release has been successfully added to the system." << endl;
        cout << "ENTER <0> to go back to the main menu: ";

        // loop to get user input selection upon exit 
        string input;
        do {
            getline(cin, input);
        } while (input != "0");
    }

    // -------------------------------------------------------------------------------------------------------------------
    // assessNewChangeControl: Scenario controller for assessing a change item 
    // -------------------------------------------------------------------------------------------------------------------
    void assessNewChangeControl() {
        cin.ignore();
        
        // get the change item using a helper function 
        Change* selectedChange = selectChange("", Assess);
        // make sure the return is valid 
        if (selectedChange == nullptr) return;

        int changeID = selectedChange->getchangeID();
        string productName = selectedChange->getProductName();
        

        // Select new status
        int statusSelection;
        cout << "=== Select Status ===" << endl;
        cout << "1) Assessed" << endl;
        cout << "2) Canceled" << endl;
        cin >> statusSelection;

        // Check for exit
        if(statusSelection == 0) return;

        Change::Status status = (statusSelection == 1) ? Change::Status::Assessed : Change::Status::Canceled;

        // Get new description  TODO: Fix to match user manual
        string description;
        cout << "ENTER a new description for the change [max 30 characters, leave blank to skip]" << endl;
        cout << "OR <0> to abort and exit to main menu:";
        getline(cin >> ws, description); // `ws` is used to ignore leading whitespace
        // Check for exit
        if(description == "0") return;

        // Select product release
        int productReleaseID = selectProductReleaseID(productName, Assess);
        if (productReleaseID == 0) return;

        // confirm
        string confirmSel;
        do {
            cout << "=== Assessed Change Item Information ===" << endl;
            cout << "Product: " << productName << endl;
            cout << "Description: " << description << endl;
            cout << "Anticipated Release: " << productReleaseID << endl;
            cout << "Status: " << Change::statusToString(status) << endl;
            cout << "Change ID: " << changeID << endl;
            cout << "ENTER <1> to confirm OR <0> to abort and exit to main menu: ";

            getline(cin, confirmSel);

            if (confirmSel == "0") return; // abort
            else if (confirmSel != "1") clearScreenAndShowError("Invalid Input");
            cout << endl;
        } while (confirmSel != "1");

        // TODO: UPDATE RECORDS
        // vector<Change*> changes = changeIO.readNRecords(maxRecordOutput);
        // Change* selectedChange = selectChange(productName, Update);
        // make relevant changes
        selectedChange->setStatus(status);
        if(!isBlank(description)) selectedChange->setDescription(description.c_str());
        if(productReleaseID != -1) selectedChange->setAnticipatedReleaseID(productReleaseID);

        changeIO.updateRecord(getChangeIndex(*selectedChange), *selectedChange);

        // Free memory
        delete selectedChange;
    }

    // -------------------------------------------------------------------------------------------------------------------
    // updateChangeItemControl: scenario controller for updating a change item 
    // -------------------------------------------------------------------------------------------------------------------
    void updateChangeItemControl() {
        cin.ignore();

        // Fetch initial product list
        vector<Product*> products = productIO.readNRecords(maxRecordOutput);
        Product* selectedProduct = selectProduct(Blank);
        
        if (!selectedProduct) return;  // Abort if no valid product selected

        string productName = selectedProduct->getProductName();
        delete selectedProduct;  // Free memory

        // Fetch initial change list
        vector<Change*> changes = changeIO.readNRecords(maxRecordOutput);
        Change* selectedChange = selectChange(productName, Update);
        
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
        int productReleaseID = selectProductReleaseID(productName, Update);

        if (productReleaseID == 0) return;  // Abort if no valid product release selected

        // TODO: do while loop here
        string description;
        cout << "ENTER a new description for the change [max 30 characters, leave blank to skip]" << endl;
        cout << "OR <0> to abort and exit to main menu:";
        getline(cin >> ws, description); // `ws` is used to ignore leading whitespace
        // Check for exit
        if(description == "0") return;

        // TODO: FIX THIS
        // vector<Change*> changes = changeIO.readNRecords(maxRecordOutput);
        // Change* selectedChange = selectChange(productName, Update);
        // Change newChange(status, productName, productReleaseID, description, dateToInt(selectedChange->getDate()));

        // update information
        selectedChange->setStatus(status);
        if(!isBlank) selectedChange->setDescription(description.c_str());
        if(productReleaseID != -1) selectedChange->setAnticipatedReleaseID(productReleaseID);
        changeIO.updateRecord(getChangeIndex(*selectedChange), *selectedChange);

        // Output updated change information
        cout << "=== Updated Change Information ===" << endl;
        cout << "Product: " << productName << endl;
        cout << "Description: " << description << endl; // TODO: REPLACE PLACEHOLDER
        cout << "Anticipated Release: " << productReleaseID << endl;
        cout << "Status: " << Change::statusToString(status) << endl;
        cout << "Change ID: " << changeID << endl;
    }

    // -------------------------------------------------------------------------------------------------------------------
    // inquireChangeItemControl: scenario controller for inquiring a change item 
    // -------------------------------------------------------------------------------------------------------------------
    void inquireChangeItemControl() {
        clearScreen();

        // select product
        vector<Product*> products = productIO.readNRecords(productIO.getRecordCount());
        if (products.empty()) {
            cout << "No products available for inquiry." << endl;
            return;
        }

        // use select product helper function to get the product
        Product* selectedProduct = selectProduct(Blank);

        if (!selectedProduct) return;  // Abort if no valid product selected
        string productName = selectedProduct->getProductName();
        delete selectedProduct;

        clearScreen();

        // Select a change item to inquire
        vector<Change*> changes = changeIO.readNRecords(changeIO.getRecordCount());
        Change* selectedChange = selectChange("", Blank);

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

        // loop to get user input selection for continuation 
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


    // -------------------------------------------------------------------------------------------------------------------
    // printScenario1Control: scenario controller for printing the change items for a specific product release 
    // -------------------------------------------------------------------------------------------------------------------
    void printScenario1Control() {
        
        // select the product using a helper function
        Product* selectedProduct = selectProduct(Blank);

        if (!selectedProduct) return;  // Abort if no valid product selected

        // get the name 
        string productName = selectedProduct->getProductName();
        delete selectedProduct;

        // select the product release using a helper function
        int selectedRelease = selectProductReleaseID(productName, Blank);

        if (selectedRelease == 0) {
            return;  // Abort if no valid product release selected
        }

        // Call PrintController with selected product and release
        PrintController::initPrintController(); 
        PrintController::printProduct(productName, selectedRelease);
        PrintController::exitPrint(); 
    }

    // -------------------------------------------------------------------------------------------------------------------
    // printScenario2Control: scenario controller for printing completed change item for a specific product release 
    // -------------------------------------------------------------------------------------------------------------------
    void printScenario2Control() {

        // select the product using a helper function
        Product* selectedProduct = selectProduct(Blank);
        // make sure the return is valid 
        if(selectedProduct==nullptr) return;

        // get the name
        string productName = selectedProduct->getProductName();
        delete selectedProduct;

        // select completed change
        Change* change = selectChange(productName, P2Control);

        // Call PrintController with selected change item
        PrintController::initPrintController(); 
        PrintController::printCompletedChangeItems(*change); 
        PrintController::exitPrint(); 

    }
}