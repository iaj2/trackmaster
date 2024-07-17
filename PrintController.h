

/*

PrintController.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin
Rev. 2 - 2024/07/16 Function implementations added

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

#include "Change.h"
#include "ProductRelease.h"
#include "FileNotOpenException.h"
#include "FileOpenFailedException.h"


namespace PrintController {


    // --------------------------------------------------------------------------------------------------------------------

    /* This function initializes the print controller

    Parameters: None

    Return: None
    */
    static void initPrintController();

    // --------------------------------------------------------------------------------------------------------------------

    /* This function prints the product information (will be called by ScenarioController)

    Parameters: None

    Return: None
    */
    static void printProduct(const ProductRelease &outputProductRelease);

    // --------------------------------------------------------------------------------------------------------------------

    /* This function prints the completed change items.

    Parameters: None

    Return: None
    */
    static void printCompletedChangeItems(const Change &outputChangeItem);

    // --------------------------------------------------------------------------------------------------------------------

    /* This function exits the print controller.

    Parameters: None

    Return: None
    */
    static void exitPrint();

    // --------------------------------------------------------------------------------------------------------------------

};

#endif