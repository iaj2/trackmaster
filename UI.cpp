#include "UI.h"
#include "ScenarioController.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ScenarioController;


namespace UI {
    void initUI() {
        
        int main_choice;
        int create_choice;
        int update_choice;
        int print_choice;

        do {
            mainMenu();
            cout << "Enter Selection [0-5]: " << endl;
        
            cin >> main_choice;
            
            switch (main_choice) {
                // Create Sub-Menu
                case 1:
                    createMenu();
                    cout << "Enter selection number [0-4]: ";
                    cin >> create_choice;
                    clearScreen();

                    // Create Choices
                    switch (create_choice) {

                        // Exit 
                        case 0:
                            break;

                        // Create Request
                        case 1:
                            createRequestControl();
                            break;

                        // Create Requester
                        case 2:
                            createRequesterControl();
                            break;

                        // Create Product
                        case 3:
                            createProductControl();
                            break;

                        // Create Product Release
                        case 4:
                            createProductReleaseControl();
                            break;
                    }
                    break;

                // Update Sub-Menu
                case 2:
                    updateMenu();
                    cout << "Enter selection number [0-2]: ";
                    cin >> update_choice;
                    clearScreen();

                    // Update Choices
                    switch (update_choice) {

                        // Exit
                        case 0:
                            break;

                        // Assess new change items
                        case 1:
                            assessNewChangeControl();
                            break;

                        // Update change item
                        case 2:
                            updateChangeItemControl();
                            break;

                    }
                    break;

                // Inquire
                case 3:
                    inquireChangeItemControl();
                    break;

                // Print Sub-Menu
                case 4:
                    printMenu();
                    cout << "Enter selection [0-2]: ";
                    cin >> print_choice;
                    clearScreen();

                    // Print Choices
                    switch (print_choice) {

                        // Exit
                        case 0:
                            break;

                        // Print Report of Upcoming Release Changes
                        case 1:
                            printScenario1Control();
                            break;

                        // Print Report of Change Requesters to Notify
                        case 2:
                            printScenario2Control();
                            break;

                    }
                    break;

                // Backup
                case 5:
                    backupControl();
                    break;

                // Exit TrackMaster
                case 0:
                    cout << "Thank you for using TrackMaster" << endl;
                    break;
            }
        } while (main_choice != 0);
    }



    void mainMenu() {
        // start on a blank screen
        clearScreen();
        cout << "    === MAIN MENU ===" << endl;
        cout << "1) Create" << endl;
        cout << "2) Update" << endl;
        cout << "3) Inquire" << endl;
        cout << "4) Print" << endl;
        cout << "5) Backup" << endl;
        cout << "0) Quit TrackMaster" << endl;
    }


    void createMenu() {
        // start on a blank screen
        clearScreen();
        cout << "    === Create Menu ===" << endl;
        cout << "1) Create Request" << endl;
        cout << "2) Create Requester" << endl;
        cout << "3) Create Product" << endl;
        cout << "4) Create Product Release" << endl;
        cout << "0) Main Menu" << endl;
    }


    void updateMenu() {
        // start on a blank screen
        clearScreen();
        cout << "    === Update Menu ===" << endl;
        cout << "1) Assess new change items" << endl;
        cout << "2) Update a change item" << endl;
        cout << "0) Main Menu" << endl;
    }


    void printMenu() {
        // start on a blank screen
        clearScreen();
        cout << "        === Print Menu ===" << endl;
        cout << "1) Print a Report of Upcoming Release Changes" << endl;
        cout << "   Description: A list of all assessed changes for a specific product" << endl;
        cout << "   that are anticipated to be included in an upcoming release and have" << endl;
        cout << "   not been completed or canceled." << endl;

        cout << endl;

        cout << "2) Print a Report of Change Requesters to Notify" << endl;
        cout << "   Description: A list of all customers and staff who need to be" << endl;
        cout << "   notified about the completion of a specific change and the release in" << endl;
        cout << "   which the change is or will be available." << endl;

        cout << endl;

        cout << "0) Exit" << endl;
    }

    void clearScreen() {
        system("clear");
    }
}
