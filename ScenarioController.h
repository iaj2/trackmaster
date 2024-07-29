
/*

ScenarioController.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/06/29 Original by Bowen Jin and Isaac James

--------------------------------------------------------------------------------------------------------------------

This module contains the class for the "ScenarioController" entity. It encapsulates the functionalities for managing certain user scenarios offered by the operations of TrackMaster such as creating a new Request or assessing change items. This class is meant to be a controller for managing various user interactions specified in our use cases.

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef SCENARIOCONTROLLER_H
#define SCENARIOCONTROLLER_H

namespace ScenarioController {

// --------------------------------------------------------------------------------------------------------------------
    
    /* Handles the user choice of creating a new Request.
    
    Parameters: None
    
    Return: Does not return anything.
    */
    void createRequestControl();

// --------------------------------------------------------------------------------------------------------------------

    /* Handles the user choice of creating a new Requester.
    
    Parameters: None
    
    Return: Does not return anything.
    */
    void createRequesterControl();

// --------------------------------------------------------------------------------------------------------------------
    
    /* Handles the user choice of creating a new Product.

    Parameters: None
    
    Return: Does not return anything.
    */
    void createProductControl();

// --------------------------------------------------------------------------------------------------------------------

    /* Handles the user choice of creating a new Product Release.

    Parameters: None
    
    Return: Does not return anything.
    */
    void createProductReleaseControl();

// --------------------------------------------------------------------------------------------------------------------


    /* Handles the user choice of assessing new change items.

    Parameters: None
    
    Return: Does not return anything.
    */
    void assessNewChangeControl();

// --------------------------------------------------------------------------------------------------------------------

    /* Handles the user choice of updating a change item.

    Parameters: None
    
    Return: Does not return anything.
    */
    void updateChangeItemControl();

// --------------------------------------------------------------------------------------------------------------------

    /* Handles the user choice of inquiring a change item.

    Parameters: None
    
    Return: Does not return anything.
    */
    void inquireChangeItemControl();

// --------------------------------------------------------------------------------------------------------------------

    /* Handles the user choice of choosing to print report of upcoming release changes

    Parameters: None
    
    Return: Does not return anything.
    */
    void printScenario1Control();

// --------------------------------------------------------------------------------------------------------------------

    /* Handles the user choice of choosing to print report of change requesters to notify
    
    Parameters: None
    
    Return: Does not return anything.
    */
     void printScenario2Control();

// --------------------------------------------------------------------------------------------------------------------

    /* Handles the user choice of backing up.
    
    Parameters: None
    
    Return: Does not return anything.
    */
    void backupControl();

// --------------------------------------------------------------------------------------------------------------------
}

#endif // SCENARIOCONTROLLER_H
