

/*

PrintController.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin

--------------------------------------------------------------------------------------------------------------------

This module encapsulates the functionality of the PrintController class, which:
1. Stores specified objects/records that the user wishes to print
2. Writes them together onto a text file
3. Prints

Please note that the class assumes proper initialization and configuration of external printing resources
(that are beyong its scope, such as printers)

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef PRINTCONTROLLER_H
#define PRINTCONTROLLER_H


namespace PrintController {

    // --------------------------------------------------------------------------------------------------------------------

    /* This function initializes the print controller

    Parameters: None

    Return: None
    */
    void initPrintController();

    // --------------------------------------------------------------------------------------------------------------------

    /* This function prints the product information (will be called by ScenarioController)

    Parameters: None

    Return: None
    */
    void printProduct();

    // --------------------------------------------------------------------------------------------------------------------

    /* This function prints the completed change items.

    Parameters: None

    Return: None
    */
    void printCompletedChangeItems();

    // --------------------------------------------------------------------------------------------------------------------

    /* This function exits the print controller.

    Parameters: None

    Return: None
    */
    void exitPrint();

    // --------------------------------------------------------------------------------------------------------------------

};

#endif