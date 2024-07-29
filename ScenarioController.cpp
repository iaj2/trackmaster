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
#include <algorithm>
#include <cctype>
#include <functional>

using namespace std;
using namespace UI;

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

// count customer type requesters
int countCustomers() {
    int count = 0;
    requesterIO.seekToStart();

    while (true) {
        Requester* requester = requesterIO.readRecord();

        // if department is empty, must be a customer
        if (requester != nullptr && requester->getDepartment() == "") {
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
        if (requester != nullptr && requester->getDepartment() != "") {
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

        // if department is empty, must be a customer
        if (requester != nullptr && requester->getDepartment() == "") {
            customers.push_back(requester);
            count++;
        } else {
            break; // If readRecord returns nullptr, stop reading
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

        // if department is not empty, must be an employee
        if (requester != nullptr && requester->getDepartment() != "") {
            employees.push_back(requester);
            count++;
        } else {
            break; // If readRecord returns nullptr, stop reading
        }
    }

    return employees;
}

// gets n product releases by product name
vector<ProductRelease*> fetchNProductReleases(int n, int recordIndex, string productName) {
    vector<ProductRelease*> prs = {nullptr};
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

        if (change != nullptr) {
            if ((productName != "" && productName == change->getProductName()) || productName == "") {
                if ((status != "" && status == Change::statusToString(change->getStatus())) || status == "") {
                changeItems.push_back(change);
                count++;
                }
            }
        } else {
            break;
        }
    }
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

    cout << productRels.size() << endl;

    while (true) {
        cout << "=== Select Product Release ===" << endl;

        // print rows
        for(int i=0; i < productRels.size(); i++) {
            if (productRels[i] == nullptr) cout << "Record unavailable" << endl;
            else {
                cout << to_string(productRels[i]->getReleaseID()) << endl;
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

    while (true) {
        cout << "=== Select Product ===" << endl;

        // print rows
        for(int i=0; i < maxRecordOutput; i++) {
            if (products[i] == nullptr) cout << "Record unavailable" << endl;
            else {
                cout << to_string(recordIndex+i+1) << ") " << products[i]->getProductName() << endl;
            }
        }
        // Option to create a new item
        if (state== Create && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(recordCount + 1) << ") New Product" << endl; 
        }

        // print options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, recordCount));

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
                if(isValidIntegerInRange(selection, 0, recordCount)){
                    option = stoi(selection);
                    if (option == 0) return nullptr;
                    if (state== Create && option  == recordCount + 1) {
                        // Handle creating a new item
                        selectedProduct = createNewProduct();
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
    // start of beginning of records
    int recordIndex = 0;
    requesterIO.seekToStart();

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

    while (true) {
        cout << "=== Select";
        if (type=="c") cout << "Customer";
        else cout << "Employee";
        cout << " ===" << endl;

        // print rows
        for(int i=0; i < requesters.size(); i++) {
            if (requesters[i] == nullptr) cout << "Record unavailable" << endl;
            else {
                cout << requesters[i]->getName() << "   " << requesters[i]->getRequesterEmail();
                // if employee, display department
                if(type=="e") cout << "   " << requesters[i]->getDepartment();
                cout << endl;
            }
        }
        // Option to create a new item
        if (state == Create  && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(recordCount + 1) << ") New ";
            if (type == "c") cout << "Customer" << endl;
            else cout << "Employee" << endl;
        }

        // print options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, recordCount));

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
            if(isValidIntegerInRange(selection, 0, recordCount)){
                option = stoi(selection);
                if (option == 0) return nullptr;
                if (state==Create  && option  == recordCount + 1) {
                    // Handle creating a new item
                    selectedRequester = createNewRequester();
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
    // start at beginning of records
    int recordIndex = 0;
    changeIO.seekToStart();

    // filter based on status
    string statusFilter;
    if (state == Assess) statusFilter = "Open";
    else if (state == Update) statusFilter = "Assessed";
    else if (state == P2Control) statusFilter = "Done";
    else statusFilter == "";

    // get first set of records
    vector<Change*> changes = fetchNChangeItems(maxRecordOutput, recordIndex, productName, statusFilter);

    // Get record count
    int recordCount = countChangeItems(productName, statusFilter);

    //
    string selection;
    int option;
    Change* selectedChange;

    while (true) {
        cout << "=== Select Change Item ===" << endl;

        // print rows
        for(int i=0; i < changes.size(); i++) {
            if (changes[i] == nullptr) cout << "Record unavailable" << endl;
            else {
                // create display
                if(state == Create ){
                    cout << changes[i]->getDescription() << "   " << to_string(changes[i]->getchangeID());
                } 
                // assess display
                else if (state == Assess || state == P2Control){
                    cout << changes[i]->getDescription() << "   ";
                    cout << Change::statusToString(changes[i]->getStatus()) << "   " << to_string(changes[i]->getchangeID());
                }      
                // update display
                else {
                    cout << changes[i]->getProductName() << "   " << changes[i]->getDescription();
                    cout << "   " << Change::statusToString(changes[i]->getStatus());
                }
            }
        }

        // Option to create a new item
        if (state==Create  && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(recordCount + 1) << ") New Change Item" << endl; 
        }

        // print options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, recordCount));

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
            if (isValidIntegerInRange, selection, 0, recordCount) {
                option = stoi(selection);

                if (option == 0) return nullptr;

                if (state==Create  && option  == recordCount + 1) {
                    // Handle creating a new item
                    selectedChange = createNewChangeItem(productName);
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

// USE CASES

// Use case 1: Create Requester
namespace ScenarioController {
    void createRequestControl() {
        // Step 3: Get requester type from user
        string reqTSelection;
        do {
            cout << "Is this request coming from a customer or an employee?" << endl;
            cout << "ENTER a selection [c - customer/e - employee]" << endl;
            cout << "OR ENTER <0> to abort and exit to the main menu" << endl;

            getline(cin, reqTSelection); 
            
            if (reqTSelection == "0") return;
            else if (reqTSelection != "c" && reqTSelection != "e") {
                clearScreenAndShowError("Invalid Input");
            }

        } while (reqTSelection != "c" && reqTSelection != "e");

        clearScreen();

        // Step 4: Get requester from user
        Requester* requester = selectRequester(scenarioState::Create, reqTSelection);

        if (requester == nullptr) return;

        string requesterName = requester->getName();
        string requesterEmail = requester->getRequesterEmail();

        delete requester;

        // Step 5: Get date information
        string date;
        do {
            cout << "ENTER the DATE of the request (YYYY-MM-DD) OR ENTER <0> to abort and" << endl;
            cout << "exit to the main menu:";

            getline(cin, date);

            if (date == "0") return;

        } while (date.empty());

        // Step 6: Get product information
        Product* product = selectProduct(scenarioState::Create);
        if (product == nullptr) return;
        string productName = product->getProductName();
        delete product;

        // Step 7: get product release id
        int productReleaseID = selectProductReleaseID(productName, scenarioState::Create);
        if (productReleaseID == 0) return;

        // Step 8: get priority
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

        // Step 9 (and 10-11 if creating new change): get change information
        Change* change = selectChange(productName, Create);
        if (change == nullptr) return;
        int changeID = change->getchangeID();
        string changeDate = change->getDate();
        string changeDesc = change->getDescription();
        delete change;

        // Step 12: confirm new request
        string confirmSel;
        string reqType = (reqTSelection == "c") ? "customer" : "employee";

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

    // Use case 2: Create Requester
    void createRequesterControl() {
        // Step 2-6
        Requester* newRequester = createNewRequester();

        if (newRequester == nullptr) return;

        requesterIO.appendRecord(*newRequester);
        delete newRequester; // free memory

        // Step 7: confirm
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

    // Use case 3: create product
    void createProductControl() {
        // step 3
        Product* newProduct = createNewProduct();
        
        if (newProduct == nullptr) return;

        productIO.appendRecord(*newProduct);
        delete newProduct; // free memory

        // step 4: confirm
        string input;
        do {
            cout << "The new product has been successfully added to the system." << endl;
            cout << "ENTER <0> to go back to the main menu: ";
            getline(cin, input);

            if(input == "0") return;

            clearScreenAndShowError("Invalid Input.");
        } while (true);
    }

    void createProductReleaseControl() {
        
        ProductRelease* newProductRelease = createNewProductRelease();
        
        if (newProductRelease == nullptr) return;
        
        productReleaseIO.appendRecord(*newProductRelease);

        cout << "The new product release has been successfully added to the system." << endl;
        cout << "ENTER <0> to go back to the main menu: ";
        string input;
        do {
            getline(cin, input);
        } while (input != "0");
    }

    // Use case 4: Assess new change items
    void assessNewChangeControl() {
        // Step 3: Select a change item
        Change* selectedChange = selectChange("", Assess);
        if (selectedChange == nullptr) return;

        int changeID = selectedChange->getchangeID();
        string productName = selectedChange->getProductName();
        
        // Free memory
        delete selectedChange;

        // Step 4: Select new status
        int statusSelection;
        cout << "=== Select Status ===" << endl;
        cout << "1) Assessed" << endl;
        cout << "2) Canceled" << endl;
        cin >> statusSelection;

        // Check for exit
        if(statusSelection == 0) return;

        Change::Status status = (statusSelection == 1) ? Change::Status::Assessed : Change::Status::Canceled;

        // Step 5: Get new description  TODO: Fix to match user manual
        string description;
        cout << "ENTER a new description for the change [max 30 characters, leave blank to skip]" << endl;
        cout << "OR <0> to abort and exit to main menu:";
        getline(cin >> ws, description); // `ws` is used to ignore leading whitespace
        // Check for exit
        if(description == "0") return;

        // Step 6: Select product release
        int productReleaseID = selectProductReleaseID(productName, Assess);
        if (productReleaseID == -1) return;

        // Step 7: confirm
        string confirmSel;
        do {
            cout << "=== Updated Change Information ===" << endl;
            cout << "Product: " << productName << endl;
            cout << "Description: " << description << endl;
            cout << "Anticipated Release: " << productReleaseID << endl;
            cout << "Status: " << Change::statusToString(status) << endl;
            cout << "Change ID: " << changeID << endl;

            getline(cin, confirmSel);

            if (confirmSel == "0") return; // abort
            else if (confirmSel != "1") clearScreenAndShowError("Invalid Input");
            cout << endl;
        } while (confirmSel != "1");

        // TODO: UPDATE RECORDS
        // changeIO.updateRecord(selectedChange);
        
    }

    void updateChangeItemControl() {
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

        // TODO: GET DESCRIPTION

        // TODO: UPDATE RECORDS

        // Output updated change information
        cout << "=== Updated Change Information ===" << endl;
        cout << "Product: " << productName << endl;
        cout << "Description: " << "THIS IS A PLACEHOLDER" << endl; // TODO: REPLACE PLACEHOLDER
        cout << "Anticipated Release: " << productReleaseID << endl;
        cout << "Status: " << Change::statusToString(status) << endl;
        cout << "Change ID: " << changeID << endl;
    }


    void inquireChangeItemControl() {
        clearScreen();

        // Step 1: select product
        vector<Product*> products = productIO.readNRecords(productIO.getRecordCount());
        if (products.empty()) {
            cout << "No products available for inquiry." << endl;
            return;
        }

        Product* selectedProduct = selectProduct(Blank);

        if (!selectedProduct) return;  // Abort if no valid product selected
        string productName = selectedProduct->getProductName();
        delete selectedProduct;

        clearScreen();

        // Step 2: Select a change item to inquire
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

        Product* selectedProduct = selectProduct(Blank);

        if (!selectedProduct) return;  // Abort if no valid product selected

        string productName = selectedProduct->getProductName();
        delete selectedProduct;

        int selectedRelease = selectProductReleaseID(productName, Blank);

        if (selectedRelease == 0) {
            return;  // Abort if no valid product release selected
        }

        // Call PrintController method with selected product and release
        PrintController::printProduct(productName, selectedRelease);
    }

    // Updated printScenario2Control using getProductRecords
    void printScenario2Control() {
        // Select product
        Product* selectedProduct = selectProduct(Blank);
        if(selectedProduct==nullptr) return;

        string productName = selectedProduct->getProductName();
        delete selectedProduct;

        // select completed change
        Change* change = selectChange(productName, P2Control);

    }
}