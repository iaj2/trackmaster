/* 
RecordNotFoundException.cpp
--------------------------------------------------------------------------------------------------------------------
Rev. 1 - 2024/07/12 Original by Aryan Saadati
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related to the header file 
*/


#include "RecordNotFoundException.h"  

// Constructor
RecordNotFoundException::RecordNotFoundException(const string& message): 
logic_error("RecordNotFoundException: " + message) {}


