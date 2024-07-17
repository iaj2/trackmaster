#include "ScenarioController.h"
#include "Change.h"
#include "Requester.h"
#include "Request.h"
#include "Product.h"
#include "UI.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace UI;

//
static const int maxRecordOutput = 5;

void clearScreen() {
    system("clear");
}

Requester** getNRequesters(const int n) {
    int i = 0;
    Requester* currentRequester = nullptr;
    Requester** requesters = new Requester*[n]; // Allocate array of Requester* pointers

    while (i < n && (currentRequester = Requester::getRequesterRecord()) != nullptr) {
        requesters[i] = currentRequester;
        i++;
    }

    return requesters;
}

Product** getNProducts(const int n) {
    int i = 0;
    Product* currentProduct = nullptr;
    Product** products = new Product*[n]; // Allocate array of Product* pointers

    while (i < n && (currentProduct = Product::getProductRecord()) != nullptr) {
        products[i] = currentProduct;
        i++;
    }

    return products;
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
    int requesterRecordOffset = 0;
    string range;
    // seek to start of file and get first set of items
    Requester::startOfRequesterFile();
    Requester** requesters = getNRequesters(maxRecordOutput);
    string requesterSelection;
    int option;
    string requesterEmail;
    
    while(true) {
        range = formatSelectionRange(1, Requester::getRequesterCount());
        cout << "=== Select Customer ===" << endl;
        // list items
        for(int i = 0; i < maxRecordOutput; i++) {
            cout << " Name" << "        Email" << endl;
            cout << to_string(requesterRecordOffset+i+1) << ")";
            if (requesters[i] == nullptr) {
                cout << "Record unavailable" << endl;
            }
            else {
                cout << requesters[i]->getName() << "        " << \
                requesters[i]->getRequesterEmail() << endl;
            }
        }
        // if list of records too long
        if (maxRecordOutput < Requester::getRequesterCount()) {
            cout << "*..." << endl;
            if (requesterRecordOffset > maxRecordOutput) {
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
                // save requester email
                requesterEmail = requesters[option-requesterRecordOffset-1]->getRequesterEmail();
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
            if (requesterSelection == "p" && requesterRecordOffset - maxRecordOutput >= 0) {
                // move pointer and get previous set of items
                requesterRecordOffset -= maxRecordOutput;
                Requester::seekRequesterFile(requesterRecordOffset);
                requesters = getNRequesters(maxRecordOutput);
            }
            else if (requesterSelection == "n" && requesterRecordOffset + maxRecordOutput <= Requester::getRequesterCount()) {
                // move pointer and get next set of items
                requesterRecordOffset += maxRecordOutput;
                Requester::seekRequesterFile(requesterRecordOffset);
                requesters = getNRequesters(maxRecordOutput);
            }
            else {
                cout << "Error: Input is invalid. Re-enter input" << endl;
                cout << "Enter 0 to abort and return to the main menu" << endl << endl;
            }
            clearScreen();
    }

    // free memory
    for (int i = 0; i < sizeof(requesters)/sizeof(Requester); i++) {
                delete requesters[i];
            }
            delete[] requesters;
        }
    
    // get date information
    string date;
    int productRecordOffset = 0;
    do {
        cout << "ENTER the DATE of the request (YYYY-MM-DD) OR ENTER <0> to abort and" << endl;
        cout << "exit to the main menu:";

        getline(cin, date);

        // check for exit
        if(date == "0") return;

    } while(date != "\0");
    
    // TODO: get product information
    Product** products = getNProducts(maxRecordOutput);
    string productSelection;
    while (true) {
        range = formatSelectionRange(1, Product::getProductCount());
        cout << "=== Select Product ===" << endl;
        for(int i = 0; i< maxRecordOutput; i++) {
            cout << to_string(productRecordOffset+i+1) << ")";
            if (products[i] == nullptr) {
                cout << "Record unavailable" << endl;
            }
            else {
                cout << products[i]->getProductName() << endl;
            }
        }
        cout << "ENTER selection" << range << "OR <0> to abort and exit to the main menu:";

        getline(cin, productSelection);

        try {
            // Attempt to convert string to integer using stoi
            option = stoi(productSelection);

            // check for exit
            if (option == 0) return;

            // check selected valid option
            if(option > 0 && option <= Product::getProductCount()) {
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
            if (productSelection == "p" && productRecordOffset - maxRecordOutput >= 0) {
                // move pointer and get previous set of items
                productRecordOffset -= maxRecordOutput;
                Requester::seekProductFile(productRecordOffset);
                products = getNProducts(maxRecordOutput);
            }
            else if (requesterSelection == "n" && productRecordOffset + maxRecordOutput <= Product::getProductCount()) {
                // move pointer and get next set of items
                productRecordOffset += maxRecordOutput;
                Requester::seekProductFile(productRecordOffset);
                products = getNProducts(maxRecordOutput);
            }
            else {
                cout << "Error: Input is invalid. Re-enter input" << endl;
                cout << "Enter 0 to abort and return to the main menu" << endl << endl;
            }
            clearScreen();
    }
    
    // TODO: get product release information

    // get priority
    Request::Priority priority;
    string prioritySelection;
  
    cout << "=== Select a Priority ===" << endl;
    cout << "1) Lowest" << endl;
    cout << "2) Low" << endl;
    cout << "3) Medium" << endl;
    cout << "4) High" << endl;
    cout << "5) Highest" << endl;
    cout << "ENTER selection [1-5] OR <0> to abort and exit to the main menu:";

    getline(cin, prioritySelection);

    // check for exit
    if(prioritySelection == "0") return;

    priority = (Request::Priority) (stoi(prioritySelection) - 1);

    // TODO: select a change item

    const int id = 123;

    // Change* newChange = new Change(
    //     id, status, productName, anticipatedreleaseID, description, date
    // )

    // Confirm new Request information to the user
    char confirm;
    do {
        cout << "=== New Request Information ===" << endl;
        cout << "Requester (" << reqTSelection << "): " << requesterEmail << endl;  // TODO: add requester name
        cout << "Product: " << endl;
        cout << "Priority: " << endl;
        cout << "Change ID: " << endl;
        cout << "ENTER <1> to confirm OR <0> to abort and exit to the main menu:" << endl;

        confirm = cin.get();

        // check for exit
        if(confirm == '0') return;

        if(confirm != '1') {
            // clear screen between attempts
            clearScreen();
           
            cout << "Error: Input is invalid. Re-enter input" << endl;
            cout << "Enter 0 to abort and return to the main menu" << endl << endl;
        }
    } while (confirm != '1');
}


