#include <iostream>
#include "Requester.h"

using namespace std;

/*
    Name of Test: Low level module test
    
    Testing read from file and write to file of binary file: "Requesters.bin" with Requester.h and Requester.cpp
    Attributes of Requester: requesterEmail (char[]), name (char[]), phone (int), departmemnt (char[])

    Preconditions: Requesters file "Requesters.bin" exists and is empty

    Scenario: 3 requesters are inserted to file, 3 requesters are read from file, compare read data and input data

*/ 

int main() {

    // Step 1
    // Write 3 example data records
    Requester Controller;
    Requester Requester1("ft@sfu.ca", "Front Tussell", static_cast<int>(7781234567), "Marketing");
    Requester Requester2("markzuck@metuh.com", "Mark Zuck", static_cast<int>(6047715734), "");
    Requester Requester3("jeffb@amoozon.com", "Jeff Bozos", static_cast<int>(2361234567), "Human Resources");
    Requester* array = new Requester[3] {Requester1, Requester2, Requester3};

    Controller.initRequester();
    Controller.recordRequester(Requester1);
    Controller.recordRequester(Requester2);
    Controller.recordRequester(Requester3);

    // Step 2
    // Read 3 example data records and compare 1 by 1
    Controller.startOfRequesterFile();
    bool pass = true;
    for (unsigned int i = 0; i < 4 ; i++) { // Run 4 times because it needs to reach end of file
        Requester* reader = Controller.getRequesterRecord();
        if (reader == NULL || i == 3) { // i cannot reach 3 because the array only has 3 objects, reader needs to read null to reach end of file
            break;
        }
        if ((reader->getRequesterEmail() != array[i].getRequesterEmail()) || (reader->getName() != array[i].getName()) || (reader->getPhone() != array[i].getPhone()) || (reader->getDepartment() != array[i].getDepartment())) {
            pass = false;
        }
    }

    // Step 3
    // Output result
    if (pass) {
        cout << "Pass" << endl;
    }
    else {
        cout << "Fail" << endl;
    }

    delete[] array;
        

    return 0;
}