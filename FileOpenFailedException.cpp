/* 
FileOpenFailedException.cpp
--------------------------------------------------------------------------------------------------------------------
Rev. 1 - 2024/07/12 Original by Aryan Saadati
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related to the header file 
*/

#include "FileOpenFailedException.h"  

// Constructor
FileOpenFailedException::FileOpenFailedException(const string& message): 
logic_error("FileOpenFailedException: " + message) {}


