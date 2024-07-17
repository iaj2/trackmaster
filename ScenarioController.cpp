#include "ScenarioController.h"
#include "Change.h"
#include "Requester.h"
#include "UI.h"
#include <iostream>
#include <cstdlib>

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

// REMOVE
int main () {
    ScenarioController::createRequestControl();
    cout << "EXITED" << endl;
    return 0;
}

