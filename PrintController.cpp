#include "PrintController.h"
#include "ProductRelease.h"
#include <fstream>

// Static file stream for handling print operations
static std::ofstream printFile;
// global variable to keep count of how many items have been printed
extern int printItemCount;

// -------------------------------------------------------------------------------------------------------------------
// Initialize the print controller
// Opens the print output file, creating it if necessary, and initializes the print item count.
// -------------------------------------------------------------------------------------------------------------------
void PrintController::initPrintController() {
    // open the print file, clean out what is inside the file 
    printFile.open("print_output.txt", std::ofstream::out | std::ofstream::trunc);
    // if the file doesn't exist make one 
    if (!printFile) {
        std::ofstream { "product.bin" };
        // Open for reading and writing
        printFile.open("product.bin", std::ios::binary);
    } 
    // throw an exception if the file open failed
    if (!printFile) {
       throw FileOpenFailedException("File open failed");
    }
    // initialize print count 
    printItemCount = 0;
}

// -------------------------------------------------------------------------------------------------------------------
// Print product information
// Generates a report for a specified product release's change items.
// -------------------------------------------------------------------------------------------------------------------
void PrintController::printProduct(const ProductRelease &outputProductRelease) {
    // check to make sure the file is open 
    if (!printFile.is_open()) {
        throw FileNotOpenException("File is not open");
    }

    // go to the begining of the change file 
    Change::startOfChangeFile();
    // make a new instance of change object 
    Change* outputChangeItem = new Change(); 

    // read the first change item 
    outputChangeItem = Change::getChangeRecord();

    // while the getChangeRecord has not returned a nullptr loop through the file 
    while(outputChangeItem != nullptr) {
        // check to see if the instance matches the Product release details 
        if (outputChangeItem->getProductName() == outputProductRelease.getProductName() && 
            outputChangeItem->getAnticipatedReleaseID() == outputProductRelease.getReleaseID()) {
            
            // Print header if its the first change item 
            if (printItemCount == 0) {
                const char * ProductName = outputChangeItem->getProductName();
                int anticipatedReleaseID = outputChangeItem->getAnticipatedReleaseID();
                printFile << "\n" << "Report for " << '"' << ProductName << '"' << " release " << '"' << anticipatedReleaseID  << '"' << "\n";
            }

            // pull all the neccesary details from the change item 
            const char * ProductName = outputChangeItem->getProductName();
            int ChangeID = outputChangeItem->getchangeID();
            char * Date = outputChangeItem->getDate();
            Status status = outputChangeItem->getStatus();
            int anticipatedReleaseID = outputChangeItem->getAnticipatedReleaseID();
            const char * Description = outputChangeItem->getDescription();

            // print it into file 
            printFile << "Change item report " << (printItemCount + 1) << ":\n";
            printFile << " Product Name: " << ProductName << "\n"; 
            printFile << " Change ID: " << ChangeID << "\n"; 
            printFile << " First Reported: " << Date << "\n"; 
            printFile << " Status: " << Change::statusToString(status) << "\n";
            printFile << " Anticipated Release: " << anticipatedReleaseID << "\n"; 
            printFile << " Description: " << Description << "\n\n\n"; 
            
            // enumarate count
            printItemCount++;
        }

        // get the next record
        outputChangeItem = Change::getChangeRecord();
    }

    // free the Change Item instance 
    free(outputChangeItem);
    return;
}

// -------------------------------------------------------------------------------------------------------------------
// Print completed change items
// Generates a report for completed change items, assuming similar logic for changes.
// -------------------------------------------------------------------------------------------------------------------
void PrintController::printCompletedChangeItems(const Change &outputChangeItem) {

    //pull all the neccesary details from the change item 
    const char * ProductName = outputChangeItem.getProductName();
    int ChangeID = outputChangeItem.getchangeID();
    char * Date = outputChangeItem.getDate();
    Status status = outputChangeItem.getStatus();
    int anticipatedReleaseID = outputChangeItem.getAnticipatedReleaseID();
    const char * Description = outputChangeItem.getDescription();

    // the header for the file 
    printFile << "\n" << "Report for " << '"' << ProductName << '"' << "\n";
    
    // print all the information into the file 
    printFile << " Product Name: " << ProductName << "\n"; 
    printFile << " Change ID: " << ChangeID << "\n"; 
    printFile << " First Reported: " << Date << "\n"; 
    printFile << " Status: " << Change::statusToString(status) << "\n"; 
    printFile << " Anticipated Release: " << anticipatedReleaseID << "\n"; 
    printFile << " Description: " << Description << "\n\n\n";

    return;
}

// -------------------------------------------------------------------------------------------------------------------
// Exit the print controller
// Closes the print file if it is open.
// -------------------------------------------------------------------------------------------------------------------
void PrintController::exitPrint() {
    if (printFile.is_open()) {
        printFile.close();
    }
}
