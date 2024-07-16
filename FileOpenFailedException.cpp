#include "FileOpenFailedException.h"  

// Constructor
FileOpenFailedException::FileOpenFailedException(const string& message): 
logic_error("FileOpenFailedException: " + message) {}


