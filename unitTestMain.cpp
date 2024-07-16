#include <iostream>
#include "ScenarioController.h"

using namespace std;
using namespace ScenarioController;

/*
    Name of Test: Functional Test
    
    Testing several functions for the header file ScenarioController.h: createRequestControl(), updateChangeItemControl(), inquireChangeItemControl()

    Preconditions: Products already exists

    Scenario: A request is created, the change item is then updated, then the user can inquire the updated create item

*/ 

int main() {

    // Step 1
    // Testing the function createRequestControl()
    cout << "Testing the function createRequestControl()" << endl;
    createRequestControl();
    // In this step, control should guide users through steps of creating request
    // If it is not aborted or encounter any errors, a new request should be created

    // Step 2
    // Testing the function updateChangeItemControl()
    cout << "Testing the function updateChangeItemControl()" << endl;
    updateChangeItemControl();
    // In this step, control should guide users through steps of updating a change item
    // If it is not aborted or encounter any errors, a change item will be updated

    // Step 3
    // Testing the function inquireChangeItemControl()
    cout << "Testing the function inquireChangeItemControl()" << endl;
    inquireChangeItemControl();
    // In this step, control should guide users through steps of inquiring a change item
    // If it is not aborted or encounter any errors, the user can inquire on the updated change item with the request

    // Expected Results
    // Based on the results while inquiring a change item, a user can determine this functions tested in this module is functional if:
    // 1. The program guides you through all processes of, create request, updating change item, inquiring change item
    // 2. The display in the inquire process shows the created and updated change item
    // If the results are different without errors in the process, the functions may not be fully functional

    // Actual Results:
    // Varies, expected results are only shown if no user errors occurred
}