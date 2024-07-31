/* 
FileNotOpenException.cpp
--------------------------------------------------------------------------------------------------------------------
Rev. 1 - 2024/07/12 Original by Aryan Saadati
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related to the header file 
*/


#include "FileNotOpenException.h"  

// Constructor
FileNotOpenException::FileNotOpenException(const string& message): 
logic_error("FileNotOpenException: " + message) {}


