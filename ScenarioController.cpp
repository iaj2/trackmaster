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

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// A list of global variables used throughout the file 
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

const int maxRecordOutput = 5; 

enum scenarioState {Create, Assess, Update, P2Control, Blank};

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 
// A list of helper functions used by the scenario controllers 
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------------------
//  formatSelectionRange: used to output the selection options to the user 
// -------------------------------------------------------------------------------------------------------------------
string formatSelectionRange(int start, int end) {
    return "[" + to_string(start) + "-" + to_string(end) + "]";
}

// -------------------------------------------------------------------------------------------------------------------
//  isValidIntegerInRange: check that a string can be converted to integer, and is within a range
// -------------------------------------------------------------------------------------------------------------------
bool isValidIntegerInRange(const string& str, int min, int max) {
    // check for input validity 
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    int value = stoi(str);
    return value >= min && value <= max;
}

// -------------------------------------------------------------------------------------------------------------------
//  clearScreenAndShowError: clears the screen and outputs an error messeage to the user
// -------------------------------------------------------------------------------------------------------------------
void clearScreenAndShowError(const string& errorMessage) {
    clearScreen();
    cout << "Error: " << errorMessage << " Please re-enter." << endl << endl;
}

// -------------------------------------------------------------------------------------------------------------------
//  isBlank: Function to check if a string is only whitespace
// -------------------------------------------------------------------------------------------------------------------
bool isBlank(const string& str) {
    return all_of(str.begin(), str.end(), [](unsigned char c) { return isspace(c); });
}

// -------------------------------------------------------------------------------------------------------------------
//  getInput: Function to get an input from the user
// -------------------------------------------------------------------------------------------------------------------
string getInput(const string& prompt, int maxLength) {
    // initialize the return value 
    string input;
    
    // loop to get user input 
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

// -------------------------------------------------------------------------------------------------------------------
//  getEmail: Function to get an email input from the user using the getInput helper function 
// -------------------------------------------------------------------------------------------------------------------
string getEmail() {
    return getInput("ENTER the EMAIL ADDRESS of the requester (Length: max 30)", Requester::MAX_EMAIL_LENGTH);
}

// -------------------------------------------------------------------------------------------------------------------
//  getPhone: Function to get a phone number input from the user  
// -------------------------------------------------------------------------------------------------------------------
string getPhone() {
    // initialize the return 
    string phoneInput;
    // loop to get user input
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

// -------------------------------------------------------------------------------------------------------------------
//  getName: Function to get a name input from the user using the getInput helper function 
// -------------------------------------------------------------------------------------------------------------------
string getName() {
    return getInput("ENTER the LAST NAME followed by the FIRST NAME of the requester (Length: max 15)", Requester::MAX_NAME_LENGTH);
}

// -------------------------------------------------------------------------------------------------------------------
//  getDepartment: Function to get a department name input from the user 
// -------------------------------------------------------------------------------------------------------------------
string getDepartment() {
    // initialize return 
    string dep;
    do {
        // Prompt the user to enter the department name
        cout << "ENTER the DEPARTMENT NAME of the requester (Length: max 12)" << endl;
        cout << "***Must be left blank if requester is not an employee of the company***" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";

        // Get input from the user
        getline(cin, dep);

        // If the user enters "0", abort and exit to the main menu
        if (dep == "0") {
            // Abort and exit to the main menu
            return "";
        }

        // Check if the input is too long
        if (dep.length() > Requester::MAX_DEPARTMENT_LENGTH) {
            clearScreenAndShowError("Input too long.");
        } 
        // Check if the input is blank or less than 1 character
        else if (isBlank(dep) || dep.length() < 1) {
            // If input is blank, set dep to an empty string and return
            dep = "";
            return dep;
        } 
        // If input is valid, return the department name
        else {
            // Valid input
            return dep;
        }

    } while (true);

    // This will never be reached, but is kept for completeness
    return dep; 
}



// -------------------------------------------------------------------------------------------------------------------
//  getProductName: Function to get a product name input from the user using the getInput helper function 
// -------------------------------------------------------------------------------------------------------------------
string getProductName() {
    // Prompt the user to enter the product name with a specified maximum length
    return getInput("ENTER the PRODUCT NAME (Length: max 10)", Product::MAX_PRODUCT_NAME_LENGTH);
}


// -------------------------------------------------------------------------------------------------------------------
//  getReleaseIDData: Function to get the release ID input from the user
// -------------------------------------------------------------------------------------------------------------------
string getReleaseIDData() {
    // Initialize the variable to store the user input
    string releaseIDInput;
    
    // Loop to get the user input
    do {
        // Prompt the user for the release ID
        cout << "ENTER the release ID of the Product Release (4 Digit integer DDDD)" << endl;
        cout << "OR ENTER <0> to abort and exit to the main menu: ";
        
        // Read the user input
        cin >> releaseIDInput;
        
        // Clear the input stream to handle any invalid inputs
        cin.clear();
        cin.ignore(10000,'\n');
        
        // Check if the user wants to abort
        if (releaseIDInput == "0") {
            return "0";
        } 
        // Check if the input length exceeds the allowed limit
        else if (releaseIDInput.length() > 4) {
            clearScreenAndShowError("Input too long.");
        }
    } while (releaseIDInput.length() > 4);  // Loop until a valid input is provided
    
    // Return the valid input
    return releaseIDInput;
}


// -------------------------------------------------------------------------------------------------------------------
//  getDateData: Function to get the date of the request from the user
// -------------------------------------------------------------------------------------------------------------------
string getDateData() {
    // Initialize the variable to store the user input
    string dateInput;

    // Loop to get the user input
    do {
        // Prompt the user for the date
        cout << "ENTER the DATE of the request (integers YYYYMMDD) OR ENTER <0> to abort and" << endl;
        cout << "exit to the main menu: ";
        
        // Read the user input
        cin >> dateInput;
        
        // Clear the input stream to handle any invalid inputs
        cin.clear();
        cin.ignore(10000, '\n');
        
        // Check if the user wants to abort
        if (dateInput == "0") {
            return "0";
        } 
        // Check if the input length exceeds the allowed limit
        else if (dateInput.length() > 8) {
            clearScreenAndShowError("Input too long.");
        }
    } while (dateInput.length() > 8);  // Loop until a valid input is provided
    
    // Return the valid input
    return dateInput;
}


// -------------------------------------------------------------------------------------------------------------------
//  countCustomers: Function to count the number of customer-type requesters
// -------------------------------------------------------------------------------------------------------------------
int countCustomers() {
    // Initialize the counter
    int count = 0;

    // Move the IO cursor to the start
    requesterIO.seekToStart();

    // Loop to read each requester record
    while (true) {
        // Read the next requester record
        Requester* requester = requesterIO.readRecord();

        // Check if the requester is a customer (department is empty)
        if (requester != nullptr && isBlank(requester->getDepartment())) {
            count++;
        } 
        // If no more records to read, exit the loop
        else if (!requester) {
            break;
        }
    }
    
    // Return the count of customer-type requesters
    return count;
}


// -------------------------------------------------------------------------------------------------------------------
//  countEmployees: Function to count the number of employee-type requesters
// -------------------------------------------------------------------------------------------------------------------
int countEmployees() {
    // Initialize the counter
    int count = 0;

    // Move the IO cursor to the start
    requesterIO.seekToStart();

    // Loop to read each requester record
    while (true) {
        // Read the next requester record
        Requester* requester = requesterIO.readRecord();

        // Check if the requester is an employee (department is not empty)
        if (requester != nullptr && !isBlank(requester->getDepartment())) {
            count++;
        } 
        // If no more records to read, exit the loop
        else if (!requester) {
            break;
        }
    }
    
    // Return the count of employee-type requesters
    return count;
}


// -------------------------------------------------------------------------------------------------------------------
//  countProductReleases: Function to count the number of releases for a given product
// -------------------------------------------------------------------------------------------------------------------
int countProductReleases(const string& productName) {
    // Initialize the counter
    int count = 0;

    // Move the IO cursor to the start
    productReleaseIO.seekToStart();

    // Loop to read each product release record
    while (true) {
        // Read the next product release record
        ProductRelease* pr = productReleaseIO.readRecord();

        // Check if the product release matches the given product name
        if (pr != nullptr && pr->getProductName() == productName) {
            count++;
        } 
        // If no more records to read, exit the loop
        else if (!pr) {
            break;
        }
    }

    // Return the count of product releases for the given product name
    return count;
}

// -------------------------------------------------------------------------------------------------------------------
//  countChangeItems: Function to count the number of change items by product name and status
// -------------------------------------------------------------------------------------------------------------------
int countChangeItems(const string& productName, const string& status = "") {
    // Initialize the counter
    int count = 0;

    // Move the IO cursor to the start
    changeIO.seekToStart();

    // Loop to read each change record
    while (true) {
        // Read the next change record
        Change* change = changeIO.readRecord();

        // Check if the change record matches the given product name and status
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

    // Return the count of change items that match the criteria
    return count;
}


// -------------------------------------------------------------------------------------------------------------------
//  getChangeIndex: Function to find the index of a specific change record by its change ID
// -------------------------------------------------------------------------------------------------------------------
int getChangeIndex(const Change selectedchange) {
    int count = 0; // Initialize the index counter
    changeIO.seekToStart(); // Start reading from the beginning of the records

    while (true) {
        Change* change = changeIO.readRecord(); // Read the next Change record

        if (change != nullptr) {
            // Check if the current record's changeID matches the selectedChange's changeID
            if (change->getchangeID() == selectedchange.getchangeID()) {
                return count; // Return the index of the matching record
            }
        } else {
            break; // Stop if no more records are available
        }
        count++; // Increment the index counter
    }
    return count; // Return the index count if the record is not found
}


// -------------------------------------------------------------------------------------------------------------------
//  dateToInt: Function to convert a date string in YYYY-MM-DD format to an integer in YYYYMMDD format
// -------------------------------------------------------------------------------------------------------------------
int dateToInt(const char* date) {
    // Convert the input C-string to a std::string
    std::string dateStr(date);

    // Remove dashes from the date string
    dateStr.erase(remove(dateStr.begin(), dateStr.end(), '-'), dateStr.end());

    // Convert the modified string to an integer
    int dateInt = std::stoi(dateStr);

    // Return the integer representation of the date
    return dateInt;
}


// -------------------------------------------------------------------------------------------------------------------
//  printListOptions: Function to display pagination and navigation options for a list of records
// -------------------------------------------------------------------------------------------------------------------
void printListOptions(int recordIndex, int recordCount, string range) {
    // If the number of records exceeds the maximum output, show an indication of more records
    if (maxRecordOutput < recordCount) cout << "*..." << endl;

    // If there are more records to display after the current page, provide an option to show the next items
    if(recordIndex + maxRecordOutput < recordCount) cout << "n) display next items" << endl;

    // If there are records displayed before the current page, provide an option to show the previous items
    if (recordIndex + 1 > maxRecordOutput) cout << "p) display previous items" << endl;

    // Prompt the user to enter a selection or to abort and exit to the main menu
    cout << "ENTER selection " << range << " OR <0> to abort and exit to the main menu: ";
}

// -------------------------------------------------------------------------------------------------------------------
//  userSelectedPrev: Determines if the user selected the option to display the next set of records
// -------------------------------------------------------------------------------------------------------------------
bool userSelectedPrev(string selection, int recordIndex) {
    // Check if the user selection is "p" (previous) and if there are previous records to show
    return (selection == "p" && recordIndex - maxRecordOutput >= 0);
}

// -------------------------------------------------------------------------------------------------------------------
//  userSelectedNext: Determines if the user selected the option to display the previous set of records
// -------------------------------------------------------------------------------------------------------------------
bool userSelectedNext(string selection, int recordIndex, int recordCount) {
    // Check if the user selection is "n" (next) and if there are more records to show
    return (selection == "n" && recordIndex + maxRecordOutput < recordCount);
}

// -------------------------------------------------------------------------------------------------------------------
//  fetchNCustomers: Retrieves 'n' customer-type requesters starting from a given index
// -------------------------------------------------------------------------------------------------------------------
vector<Requester*> fetchNCustomers(int n, int recordIndex) {
    vector<Requester*> customers; // Vector to hold customer requesters
    int count = 0; // Counter to keep track of the number of customers fetched

    // Set the position to start reading records from the given index
    requesterIO.seekTo(recordIndex);
    
    // Loop until 'n' customers are retrieved or until there are no more records
    while (count < n) {
        Requester* requester = requesterIO.readRecord(); // Read a record from the IO

        if(!requester) {
            break; // If readRecord returns nullptr, stop reading as there are no more records
        }

        // Check if the requester is a customer (i.e., department is empty)
        if (isBlank(requester->getDepartment())) {
            customers.push_back(requester); // Add the customer to the vector
            count++; // Increment the count of customers fetched
        } else {
            delete requester; // If not a customer, delete the requester object to free memory
        }
    }
    return customers; // Return the vector of customer requesters
}


// -------------------------------------------------------------------------------------------------------------------
//  fetchNEmployees: Retrieves 'n' employee-type requesters starting from a given index
// -------------------------------------------------------------------------------------------------------------------
vector<Requester*> fetchNEmployees(int n, int recordIndex) {
    vector<Requester*> employees; // Vector to hold employee requesters
    int count = 0; // Counter to keep track of the number of employees fetched

    // Set the position to start reading records from the given index
    requesterIO.seekTo(recordIndex);
    
    // Loop until 'n' employees are retrieved or until there are no more records
    while (count < n) {
        Requester* requester = requesterIO.readRecord(); // Read a record from the IO

        if (!requester) {
            break; // If readRecord returns nullptr, stop reading as there are no more records
        }

        // Check if the requester is an employee (i.e., department is not empty)
        if (!isBlank(requester->getDepartment())) {
            employees.push_back(requester); // Add the employee to the vector
            count++; // Increment the count of employees fetched
        } else {
            delete requester; // If not an employee, delete the requester object to free memory
        }
    }

    return employees; // Return the vector of employee requesters
}


// -------------------------------------------------------------------------------------------------------------------
//  fetchNProductReleases: Retrieves 'n' product release records matching the specified product name, starting from a given index
// -------------------------------------------------------------------------------------------------------------------
vector<ProductRelease*> fetchNProductReleases(int n, int recordIndex, string productName) {
    vector<ProductRelease*> prs; // Vector to hold product release records
    int count = 0; // Counter to keep track of the number of releases fetched

    // Set the position to start reading records from the given index
    productReleaseIO.seekTo(recordIndex);

    // Loop until 'n' product releases are retrieved or until there are no more records
    while (count < n) {
        ProductRelease* pr = productReleaseIO.readRecord(); // Read a record from the IO

        if (!pr) {
            // Handle the case where no more records are available
            break;
        }

        // Check if the product release matches the specified product name
        if (pr->getProductName() == productName) {
            prs.push_back(pr); // Add the matching product release to the vector
            count++; // Increment the count of releases fetched
        } else {
            // If the product name does not match, clean up the object to prevent memory leaks
            delete pr; // Delete the non-matching record to free memory
        }
    }

    return prs; // Return the vector of product release records
}


// -------------------------------------------------------------------------------------------------------------------
//  fetchNChangeItems: Fetches up to n change items starting from recordIndex
//  Filters by product name and status if provided. If status is blank, fetches change items of any status.
//  If product name is blank, does not filter by product name.
// -------------------------------------------------------------------------------------------------------------------
vector<Change*> fetchNChangeItems(int n, int recordIndex, string productName, string status = "") {
    vector<Change*> changeItems; // Vector to store the fetched change items
    int count = 0; // Counter to keep track of the number of fetched change items

    changeIO.seekTo(recordIndex); // Set the read position to the given record index

    while (count < n) {
        Change* change = changeIO.readRecord(); // Read a record from the IO

        if (!change) break; // Stop reading if no more records are available

        // Check if the product name matches the filter criteria or if the product name filter is blank
        if ((productName != "" && productName == change->getProductName()) || productName == "") {
            // Check if the status matches the filter criteria or if the status filter is blank
            if ((status != "" && status == Change::statusToString(change->getStatus())) || status == "") {
                changeItems.push_back(change); // Add the change item to the vector
                count++; // Increment the counter
            }
        } else {
            delete change; // Delete the change item to prevent memory leaks
        }
    }

    return changeItems; // Return the vector of fetched change items
}

// -------------------------------------------------------------------------------------------------------------------
//  selectProductReleaseID: Allows the user to select a product release ID from a list of product releases.
//  Displays a paginated list of product releases filtered by the given product name. Provides options to 
//  navigate through pages and select a product release ID. Returns the selected release ID, 0 to abort, 
//  or -1 to skip (when in Assess or Update state and the skip option is selected).
// -------------------------------------------------------------------------------------------------------------------
int selectProductReleaseID(string productName, scenarioState state) {
    // Initialize record index to start from the beginning
    int recordIndex = 0;

    // Fetch the first set of product release records
    vector<ProductRelease*> productRels = fetchNProductReleases(maxRecordOutput, recordIndex, productName);

    // Get the total count of product releases for the given product name
    int recordCount = countProductReleases(productName);

    string selection; // User's selection input
    int option; // Option chosen by the user
    ProductRelease* selectedProductRel; // Pointer to the selected product release

    while (true) {
        cout << "=== Select Product Release ===" << endl;

        // Print the list of product releases
        for (int i = 0; i < productRels.size(); i++) {
            if (productRels[i] != nullptr) {
                cout << to_string(recordIndex + i + 1) << ") " << to_string(productRels[i]->getReleaseID()) << endl;
            } else {
                cout << "Record unavailable" << endl;
            }
        }

        // Option to skip if in Assess or Update state and at the end of the list
        if ((state == Assess || state == Update) && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(recordCount + 1) << ") Skip" << endl;
        }

        // Print options for navigation and selection
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, recordCount));

        // Get user input
        getline(cin, selection);

        // Handle user selection for next page
        if (userSelectedPrev(selection, recordIndex)) {
            recordIndex -= maxRecordOutput; // Move to the previous page
            productRels = fetchNProductReleases(maxRecordOutput, recordIndex, productName);
        } 
        // Handle user selection for previous page
        else if (userSelectedNext(selection, recordIndex, recordCount)) {
            recordIndex += maxRecordOutput; // Move to the next page
            productRels = fetchNProductReleases(maxRecordOutput, recordIndex, productName);
        } 
        // Handle user selection for specific option or skip
        else {
            if (isValidIntegerInRange(selection, 0, recordCount)) {
                option = stoi(selection);
                if (option == 0) return 0; // Abort and exit to the main menu
                if (state == Assess && option == recordCount + 1) {
                    return -1; // Skip option
                } else {
                    selectedProductRel = productRels[option - recordIndex - 1]; // Get selected product release
                    // Optionally free memory if needed
                    // for (ProductRelease* p : productRels) delete p;  
                    return selectedProductRel->getReleaseID(); // Return the selected release ID
                }
            } else {
                clearScreenAndShowError("Option you entered does not exist on the list."); // Show error for invalid input
                cout << endl;
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------------------------
//  createNewRequester: Handles the creation of a new requester by gathering user input for email, phone number, 
//  name, and department. It clears the screen between each input prompt. Returns a pointer to a new Requester 
//  object based on the gathered user input, or nullptr if the user chooses to abort the operation.
// -------------------------------------------------------------------------------------------------------------------
Requester* createNewRequester() {
    clearScreen(); // Clear the screen before starting the input process

    // Get email from the user
    string requesterEmail = getEmail();
    if (requesterEmail == "0") return nullptr; // Abort if the user enters "0"

    clearScreen(); // Clear the screen after getting email

    // Get phone number from the user
    string phoneInput = getPhone();
    if (phoneInput == "0") return nullptr; // Abort if the user enters "0"
    int phone = stol(phoneInput); // Convert phone number from string to integer

    clearScreen(); // Clear the screen after getting phone number

    // Get name from the user
    string name = getName();
    if (name == "0") return nullptr; // Abort if the user enters "0"

    clearScreen(); // Clear the screen after getting name

    // Get department from the user
    string department = getDepartment();
    if (department == "0") return nullptr; // Abort if the user enters "0"

    clearScreen(); // Clear the screen after getting department

    // Create and return a new Requester object with the gathered user input
    Requester* newRequester = new Requester(requesterEmail.c_str(), name.c_str(), phone, department.c_str());

    return newRequester; // Return the newly created Requester object
}


// -------------------------------------------------------------------------------------------------------------------
//  createNewProduct: Handles the creation of a new product by gathering user input for the product name. Clears 
//  the screen before and after the input prompt. Returns a pointer to a new Product object based on the gathered 
//  user input, or nullptr if the user chooses to abort the operation.
// -------------------------------------------------------------------------------------------------------------------
Product* createNewProduct() {
    clearScreen(); // Clear the screen before starting the input process
    
    // Get product name from the user
    string productName = getProductName();
    if (productName == "0") return nullptr; // Abort if the user enters "0"

    clearScreen(); // Clear the screen after getting product name

    // Create and return a new Product object with the gathered user input
    Product* newProduct = new Product(productName.c_str());

    return newProduct; // Return the newly created Product object
}

// -------------------------------------------------------------------------------------------------------------------
//  createNewProductRelease: Covers steps to create a new ProductRelease object based on user input. 
//  Prompts the user for product details and validates input before creating and returning a new ProductRelease object.
// -------------------------------------------------------------------------------------------------------------------
ProductRelease* createNewProductRelease() {
    clearScreen(); // Clear the screen before user input
    
    // Get the product name
    string productName = getProductName();
    if (productName == "0") return nullptr; // Abort if user chooses to exit

    clearScreen(); // Clear the screen before the next input

    // Get the release ID
    string ReleaseID = getReleaseIDData();
    if (ReleaseID == "0") return nullptr; // Abort if user chooses to exit
    int ReleaseIDNumber = stol(ReleaseID); // Convert release ID to integer
    
    clearScreen(); // Clear the screen before the next input

    string date;
    // Loop to get a valid date input from the user
    do {
        cout << "ENTER the DATE of the release (YYYY-MM-DD) OR ENTER <0> to abort and" << endl;
        cout << "exit to the main menu:";

        getline(cin, date);

        if (date == "0") return nullptr; // Abort if user chooses to exit

        clearScreen(); // Clear the screen before validating the input

    } while (date.empty()); // Ensure date is not empty

    const char* dateChar = date.c_str(); // Convert string to C-string
    int dateNumber = dateToInt(dateChar); // Convert date to integer representation

    clearScreen(); // Clear the screen before finalizing

    // Create and return a new ProductRelease object
    ProductRelease* newProductRelease = new ProductRelease(productName.c_str(), ReleaseIDNumber, dateNumber);

    return newProductRelease;
}

// -------------------------------------------------------------------------------------------------------------------
//  createNewChangeItem: Covers steps to create a new Change object based on user input.
//  Prompts the user for change details and validates input before creating and returning a new Change object.
// -------------------------------------------------------------------------------------------------------------------
Change* createNewChangeItem(string productName) {
    clearScreen(); // Clear the screen before user input

    // Get the change description
    string changeDescription = getInput("ENTER a description for a new change (30 char. max)", Change::MAX_DESCRIPTION_LENGTH);
    if (changeDescription == "0") return nullptr; // Abort if user chooses to exit

    clearScreen(); // Clear the screen before the next input

    // Prompt the user to select a product release
    cout << "Select a product release that the change is anticipated to be completed for" << endl;
    int productRelease = selectProductReleaseID(productName, Blank);

    // TODO: Implement date input
    Change* newChangeItem = new Change(Change::Status::Open, productName.c_str(), productRelease, 
                                        changeDescription.c_str(), 1231); // Placeholder date value

    return newChangeItem; // Return the newly created Change object
}

// -------------------------------------------------------------------------------------------------------------------
// This function allows the user to select a product from a list of available products.
// Depending on the scenario state, the user may also have the option to create a new product.
// -------------------------------------------------------------------------------------------------------------------
Product* selectProduct(scenarioState state) {
    clearScreen();  // Clear the screen to start fresh

    int recordIndex = 0;  // Initialize the starting index for records

    // Start reading records from the beginning
    productIO.seekToStart();
    vector<Product*> products = productIO.readNRecords(maxRecordOutput);  // Fetch the initial set of records

    int recordCount = productIO.getRecordCount();  // Get the total number of records

    string selection;  // To hold user input
    int option;  // To hold the selected option
    Product* selectedProduct;  // To store the selected product

    // Determine the maximum selection option based on the scenario state
    int maxSelection = recordCount;
    if(state == scenarioState::Create) {
        maxSelection += 1;  // Include the option to create a new product
    }

    while (true) {
        cout << "=== Select Product ===" << endl;

        // Display the list of products
        for(int i=0; i < products.size(); i++) {
            if (products[i] != nullptr) {
                cout << to_string(recordIndex+i+1) << ") " << products[i]->getProductName() << endl;
            } else {
                cout << "Record unavailable" << endl;  // Indicate unavailable records
            }
        }

        // If in 'Create' state and at the end of the list, provide option to create a new product
        if (state== Create && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(maxSelection) << ") New Product" << endl; 
        }

        // Display navigation options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, maxSelection));

        // get user input
        getline(cin, selection);
        
        if (userSelectedPrev(selection, recordIndex)) {
            // User selected 'next', adjust recordIndex and fetch next set of records
            recordIndex -= maxRecordOutput;
            productIO.seekTo(recordIndex);
            products = productIO.readNRecords(maxRecordOutput);
            clearScreen();
        } else if (userSelectedNext(selection, recordIndex, recordCount)) {
            // User selected 'previous', adjust recordIndex and fetch previous set of records
            recordIndex += maxRecordOutput;
            productIO.seekTo(recordIndex);
            products = productIO.readNRecords(maxRecordOutput);
            clearScreen();
        } else {
            // Validate user selection
            if(isValidIntegerInRange(selection, 0, maxSelection)) {
                option = stoi(selection);
                if (option == 0) return nullptr;  // User chose to abort
                if (state== Create && option  == maxSelection) {
                    // User chose to create a new product
                    selectedProduct = createNewProduct();
                    productIO.appendRecord(*selectedProduct);  // Save the new product
                    cout << "New Product created!" << endl << endl;
                    return selectedProduct;  // Return the newly created product
                } else {
                    // Select the product based on user input
                    selectedProduct = products[option - recordIndex - 1];
                    // for (Product* product : products) delete product;  // Clean up memory if needed
                    return selectedProduct;
                }
            } else {
                clearScreenAndShowError("Invalid input.");  // Handle invalid input
            }
        }
    }
}


Requester* selectRequester(scenarioState state, string type) {
    clearScreen();  // Clear the screen for a fresh display

    int recordIndex = 0;  // Initialize the starting index for records

    // Fetch the appropriate records based on type
    vector<Requester*> requesters;
    int recordCount;

    if(type == "c"){
        requesters = fetchNCustomers(maxRecordOutput, recordIndex);  // Fetch customer requesters
        recordCount = countCustomers();  // Get total customer count
    } 
    else {
        requesters = fetchNEmployees(maxRecordOutput, recordIndex);  // Fetch employee requesters
        recordCount = countEmployees();  // Get total employee count
    }

    string selection;  // To hold user input
    int option;  // To hold the selected option
    Requester* selectedRequester;  // To store the selected requester

    // Determine the maximum selection option based on the scenario state
    int maxSelection = recordCount;
    if(state == scenarioState::Create) {
        maxSelection += 1;  // Include the option to create a new requester
    }

    while (true) {
        cout << "=== Select ";
        if (type=="c") cout << "Customer";  // Display type-specific heading
        else cout << "Employee";
        cout << " ===" << endl;

        // Print column headers
        cout << "   Name       " << "    Email";
        if(type=="e") {
            cout << "                         Department";  // Display department for employees
        }
        cout << endl;

        // Print the list of requesters
        for(int i=0; i < requesters.size(); i++) {
            if (requesters[i] != nullptr) {
                cout << to_string(recordIndex+i+1) << ") ";
                cout << requesters[i]->getName(); 

                // Align the name field
                const char * tempName = requesters[i]->getName();
                for(int j = 0; j < (Requester::MAX_NAME_LENGTH - strlen(tempName)); j++ ){
                    cout << " "; 
                }
                
                cout << requesters[i]->getRequesterEmail();

                // Align the email field
                const char * tempEmail = requesters[i]->getRequesterEmail();
                for(int j = 0; j < (Requester::MAX_EMAIL_LENGTH - strlen(tempEmail)); j++ ){
                    cout << " "; 
                }
                
                // Print department if employee
                if(type=="e") cout << requesters[i]->getDepartment();
                cout << endl;
            } 
            else {
                cout << "Record unavailable" << endl;  // Indicate unavailable records
            }
        }

        // Option to create a new requester
        if (state == Create  && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(maxSelection) << ") New ";
            if (type == "c") cout << "Customer" << endl;
            else cout << "Employee" << endl;
        }

        // Display navigation and selection options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, maxSelection));

        // Get user input
        getline(cin, selection);

        // Handle user selection for next set of items
        if (userSelectedPrev(selection, recordIndex)) {
            recordIndex -= maxRecordOutput;
            if(type == "c") { requesters = fetchNCustomers(maxRecordOutput, recordIndex); } 
            else { requesters = fetchNEmployees(maxRecordOutput, recordIndex); }
            clearScreen();
        
        // Handle user selection for previous set of items
        } else if (userSelectedNext(selection, recordIndex, recordCount)) {
            recordIndex += maxRecordOutput;
            if(type == "c") { requesters = fetchNCustomers(maxRecordOutput, recordIndex); }
            else { requesters = fetchNEmployees(maxRecordOutput, recordIndex); }
            clearScreen();
        
        // Handle user selection
        } else {
            if(isValidIntegerInRange(selection, 0, maxSelection)) {
                option = stoi(selection);
                if (option == 0) return nullptr;  // User chose to abort
                if (state==scenarioState::Create && option  == maxSelection) {
                    // Handle creating a new requester
                    selectedRequester = createNewRequester();
                    requesterIO.appendRecord(*selectedRequester);  // Save the new requester
                    cout << "New Requester created!" << endl << endl;
                    return selectedRequester;  // Return the newly created requester
                } else {
                    // Select the requester based on user input
                    selectedRequester = requesters[option - recordIndex - 1];
                    // for (Requester* req : requesters) delete req;  // Clean up memory if needed
                    return selectedRequester;
                }
            } else{
                clearScreenAndShowError("Invalid input.");  // Handle invalid input
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------------------------
// This function allows the user to select a requester from a list of available customers or employees.
// Depending on the scenario state, the user may also have the option to create a new requester.
// -------------------------------------------------------------------------------------------------------------------
Change* selectChange(string productName, scenarioState state) {
    clearScreen();  // Clear the screen for a fresh display

    int recordIndex = 0;  // Initialize the starting index for records
    changeIO.seekToStart();  // Move to the start of change records

    // Determine the status filter based on the scenario state
    string statusFilter;
    if (state == Assess) statusFilter = "Open";
    else if (state == Update) statusFilter = "Assessed";
    else if (state == P2Control) statusFilter = "Done";
    else statusFilter = "";

    // Get the first set of records based on filters
    vector<Change*> changes = fetchNChangeItems(maxRecordOutput, recordIndex, productName, statusFilter);

    // Get the total record count based on filters
    int recordCount = countChangeItems(productName, statusFilter);

    string selection;  // To hold user input
    int option;  // To hold the selected option
    Change* selectedChange;  // To store the selected change item

    // Determine the maximum selection option based on the scenario state
    int maxSelection = recordCount;
    if (state == scenarioState::Create) {
        maxSelection += 1;  // Include the option to create a new change item
    }

    while (true) {
        cout << "=== Select Change Item ===" << endl;

        // print headers
        if(state == scenarioState::Create) {
            cout << "   Description                   " << "ChangeID" << endl;
        }
        else if(state == scenarioState::Assess) {
            cout << "   Product        " << "Description                   " << "Status" << endl; 
        }
        else {
            cout << "   Description                   " << "Status     " << "ChangeID" << endl;
        }

        // Print the list of change items
        for(int i=0; i < changes.size(); i++) {
            if (changes[i] != nullptr) {
                cout << to_string(i + recordIndex + 1) << ") ";
                // Display details based on the state
                if (state == Create) {
                    cout << changes[i]->getDescription();    

                const char * tempDescription = changes[i]->getDescription();
                for(int j = 0; j < (Change::MAX_DESCRIPTION_LENGTH - strlen(tempDescription)); j++ ){
                    cout << " "; 
                }

                    cout << to_string(changes[i]->getchangeID());
                } 
                // assess display
                else if (state == Assess){
                    cout << changes[i]->getProductName(); 

                    const char * tempName = changes[i]->getProductName();
                    for(int j = 0; j < (Change::MAX_PRODUCT_NAME_LENGTH - strlen(tempName)); j++ ){
                        cout << " "; 
                    }

                    cout << changes[i]->getDescription();

                    const char * tempDescription = changes[i]->getDescription();
                    for(int j = 0; j < (Change::MAX_DESCRIPTION_LENGTH - strlen(tempDescription)); j++ ){
                        cout << " "; 
                    }

                    cout << Change::statusToString(changes[i]->getStatus());
                }      
                // update, print display
                else {
                    
                    cout << changes[i]->getDescription();

                    const char * tempDescription = changes[i]->getDescription();
                    for(int j = 0; j < (Change::MAX_DESCRIPTION_LENGTH - strlen(tempDescription)); j++ ){
                        cout << " "; 
                    }

                    cout << Change::statusToString(changes[i]->getStatus()) ;

                    const char * tempStatues = Change::statusToString(changes[i]->getStatus()).c_str();
                    for(int j = 0; j < (11 - strlen(tempStatues)); j++ ){
                        cout << " "; 
                    } 
                    
                    cout << to_string(changes[i]->getchangeID());
                }
            }
            else {
                cout << "Record unavailable";  // Indicate unavailable records
            }
            cout << endl;
        }

        // Option to create a new change item
        if (state == Create && (recordIndex + maxRecordOutput >= recordCount)) {
            cout << to_string(maxSelection) << ") New Change Item" << endl; 
        }

        // Display navigation and selection options
        printListOptions(recordIndex, recordCount, formatSelectionRange(1, maxSelection));

        // get user input
        getline(cin, selection);

        // Handle user selection for next set of items
        if (userSelectedPrev(selection, recordIndex)) {
            recordIndex -= maxRecordOutput;
            changes = fetchNChangeItems(maxRecordOutput, recordIndex, productName, statusFilter);
            clearScreen();
        
        // Handle user selection for previous set of items
        } else if (userSelectedNext(selection, recordIndex, recordCount)) {
            recordIndex += maxRecordOutput;
            changes = fetchNChangeItems(maxRecordOutput, recordIndex, productName, statusFilter);
            clearScreen();
        
        // Handle user selection
        } else {
            if (isValidIntegerInRange(selection, 0, maxSelection)) {
                option = stoi(selection);

                if (option == 0) return nullptr;  // User chose to abort

                if (state == Create && option == maxSelection) {
                    // Handle creating a new change item
                    selectedChange = createNewChangeItem(productName);
                    changeIO.appendRecord(*selectedChange);
                    cout << "New Change Item created!" << endl << endl;
                    return selectedChange;  // Return the newly created change item
                } else {
                    // Select the change item based on user input
                    selectedChange = changes[option - recordIndex - 1];
                    // for (Change* change : changes) delete change;  // Clean up memory if needed
                    return selectedChange;
                }
            } else {
                clearScreenAndShowError("Invalid input.");  // Handle invalid input
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------------------------
// Prompts the user to select a priority level from a predefined list
// -------------------------------------------------------------------------------------------------------------------
int selectPriority() {
    clearScreen();  // Clear the screen for a fresh display
    string selection;  // To hold user input
    
    do {
        cout << "=== Select a Priority ===" << endl;
        cout << "1) Lowest" << endl;
        cout << "2) Low" << endl;
        cout << "3) Medium" << endl;
        cout << "4) High" << endl;
        cout << "5) Highest" << endl;
        cout << "ENTER selection [1-5] OR <0> to abort and exit to the main menu: ";

        getline(cin, selection);  // Get user input

        // If the user chooses to abort
        if (selection == "0") {
            return -1;  // Return -1 to indicate abortion
        }

        // Validate the user input
        if (isValidIntegerInRange(selection, 1, 5)) {
            return stoi(selection);  // Convert the valid input to an integer and return it
        } else {
            clearScreenAndShowError("Invalid Input.");  // Show an error message for invalid input
        }
    } while (true);  // Continue prompting until valid input is received or user aborts
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

        // Get requester from user
        clearScreen();
        Requester* requester = selectRequester(scenarioState::Create, reqTSelection);

        if (!requester) return;

        string requesterName = requester->getName();
        string requesterEmail = requester->getRequesterEmail();

        delete requester;

        // Loop to Get date information
        clearScreen();
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
        if (!product) return;
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
        if (!change) return;
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
        if (!newRequester) return;

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
        if (!newProduct) return;

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
        if (!newProductRelease) return;
        
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
        if (!selectedChange) return;

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
        cin.ignore();
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
        if(!selectedProduct) return;

        // get the name
        string productName = selectedProduct->getProductName();
        delete selectedProduct;

        // select completed change
        Change* change = selectChange(productName, P2Control);

        if (!change) return;

        // Call PrintController with selected change item
        PrintController::initPrintController(); 
        PrintController::printCompletedChangeItems(*change); 
        PrintController::exitPrint(); 

    }
}