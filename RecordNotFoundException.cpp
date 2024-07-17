#include "RecordNotFoundException.h"  

// Constructor
RecordNotFoundException::RecordNotFoundException(const string& message): 
logic_error("RecordNotFoundException: " + message) {}


