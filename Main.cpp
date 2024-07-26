
// main.cpp

#include "UI.h"
#include "ScenarioController.h"
#include "LifeCycleController.h"
#include <iostream>

using namespace std;
using namespace LifeCycleController;
using namespace UI;

int main() {
    // Setup calls will go before UI initialized
    startUpSystem();
    
    initUI();

    // Shutdown after
    shutDownSystem();
};





/* 

Writing Conventions:
Please note that a dividing line must seperate each convention shown below. These comments will not require any indentation.

1. The Module name must be declared at the top of each module.h file.
2. Revision History must be stated first to last, represented by the Revision # - Date (YYYY/MM/DD) and changes made.
3. Module description and overview must be summarized and stated.
4. Any libraries and header files included will be stated after.

- Above and below each function, there will be a dividing line.
- Briefly describe the function prototype (its use and potentially restrictions).
- State the parameters and what it returns, each with a brief description.
The parameter descriptions will show if the parameters are inputs/outputs.
- Function names will start with a lower-case letter (excluding Constructors).
- Indentations used will be the VSCode (1.89) default indentation block size, which is 4-characters, only required after an opening brace " { ".
- Opening braces will appear on the same line that starts the function/block.
- Classes must have public members first, then protected, then private.


There are 2 following examples (ExampleModule.h and ExampleModule.cpp):


ExampleModule.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin

--------------------------------------------------------------------------------------------------------------------

This module contains the declarations for a sorting function used on integer arrays. This exported function calls on other sorting algorithms depending on the "sortedness" of the array and hides the implementation of the sorting algorithm it chooses to use. Please note that the sorting algorithms may be subject to change in the future, depending on the size of the data.

--------------------------------------------------------------------------------------------------------------------

#include "sorting.h"
using namespace std;

// Exported types and variables
enum Sorted {Yes, Partially, No};    // A flag used to indicate the "sortedness" of the array.

class ExampleSort {
    public:

    // --------------------------------------------------------------------------------------------------------------------
        This function will sort an integer array. This will call different sorting algorithms depending on the "sortedness" of the array indicated by the second parameter. For instance, the variable "Yes" will indicate that the array is sorted, which means that the function will not change the array.

        - Parameter 1 (int* arr): In/Out parameter. This is a pointer to an integer array that the caller wishes to sort.
        - Parameter 2 (Sorted sortedness): In parameter. This is a variable from the enum "Sorted" that helps the function choose the appropriate sorting algorithm.
        - Return: Does not return anything.

        void sort(int* arr, Sorted sortedness);
};
*/


/*
ExampleSort.cpp
#include "ExampleSort.h"

Reminder that...
1. Functions will start with a capital letter and start will no indentation
2. We will have the opening brace on the same line as the function name
3. Any opening brace will require an indentation (4-character block)

void ExampleSort::sort(int* arr, Sorted sortedness) {
    int i = 3;
    for (int k = 0; k < i; k++) {
        k += i;
    }
}

*/