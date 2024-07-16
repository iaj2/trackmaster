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

// Requester** getNChangeRecords(const int n) {
//     int i = 0;
//     Requester* currentRequester = nullptr;
//     Requester** requesters = new Requester*[n]; // Allocate array of Requester* pointers

//     while (i < n && (currentRequester = Requester::getRequesterRecord()) != nullptr) {
//         requesters[i] = currentRequester;
//         i++;
//     }

//     return requesters;
// }

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
            
           
            cout << "Error: Input is invalid. Re-enter input" << endl;
            cout << "Enter 0 to abort and return to the main menu" << endl << endl;
        }

        // clear screen between prompts
        clearScreen();

        // Clear stdin
        cin.ignore();

    } while(reqTSelection != 'c' && reqTSelection != 'e');


    // get requester from user
    const int maxRecordOutput = 5;
    int recordOffset = 0;
    string range;
    // Requester::startOfRequesterFile();
    // Requester** requests = getNChangeRecords(maxRecordOutput);
    string requesterSelection;
    int option;
    do {
        range = formatSelectionRange(recordOffset+1, recordOffset+maxRecordOutput);
        cout << "=== Select Customer ===" << endl;
        cout << "ENTER selection " << range << " OR <0> to abort and exit to the main menu:";

       
        getline(cin, requesterSelection);

        try {
            // Attempt to convert string to integer using stoi
            option = stoi(requesterSelection);

            

        } catch (const invalid_argument& ia) {
            cout << "Error: Input is invalid. Re-enter input" << endl;
            cout << "Enter 0 to abort and return to the main menu" << endl << endl;
        } catch (const out_of_range& oor) {
            cout << "Error: Input is invalid. Re-enter input" << endl;
            cout << "Enter 0 to abort and return to the main menu" << endl << endl;
        }

    } while ();
    
    
}



int main () {
    ScenarioController::createRequestControl();
    cout << "EXITED" << endl;
    return 0;
}

