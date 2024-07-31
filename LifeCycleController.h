
/*
LifeCycleController.h
--------------------------------------------------------------------------------------------------------------------
Revision History:
Rev. 1 - 2024/07/02 Original by Bowen Jin and Isaac James
Rev. 2 - 2024/07/30 
--------------------------------------------------------------------------------------------------------------------
This module contains the declarations for the LifeCycleController class, which manages the:
    1. Initialization
    2. Startup
    3. Shutdown
    4. Exit
processes of the system. The implementation details of these functions are hidden from the user.
This class is responsible for controlling all other modules of the system, and ensures that all necessary resources are properly managed throughout the system's lifecycle.
*/

#ifndef LIFECYCLECONTROLLER_H
#define LIFECYCLECONTROLLER_H

namespace LifeCycleController {

    // --------------------------------------------------------------------------------------------------------------------

    /* Starts up the system by opening all files necessary

    Parameters: None

    Return: None
    */
    void startUpSystem();

    // --------------------------------------------------------------------------------------------------------------------

    /* Shuts down the system by:
        1. Frees any dynamically allocated memory.
        2. Closing all files after the main menu has been exited.

    Parameters: None

    Return: None
    */
    void shutDownSystem();

    // --------------------------------------------------------------------------------------------------------------------

}

#endif // LIFECYCLECONTROLLER_H