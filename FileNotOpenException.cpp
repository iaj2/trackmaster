#include "FileNotOpenException.h"  

// Constructor
FileNotOpenException::FileNotOpenException(const string& message): 
logic_error("FileNotOpenException: " + message) {}


