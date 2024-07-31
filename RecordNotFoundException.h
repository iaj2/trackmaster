/* 
RecordNotFoundException.h
--------------------------------------------------------------------------------------------------------------------
Rev. 1 - 2024/07/12 Original by Aryan Saadati
--------------------------------------------------------------------------------------------------------------------
This class represents an exception that is thrown when a specific record is not found 
during a file operation or data retrieval process. It inherits from the std::logic_error 
class and allows for custom error messages to be specified.
*/

#ifndef RECORD_NOT_FOUND_EXCEPTION_H
#define RECORD_NOT_FOUND_EXCEPTION_H

#include <stdexcept>

using std::string;
using std::logic_error;

class RecordNotFoundException : public logic_error {
   
   public:
      // Constructor
      RecordNotFoundException(const string& message = "");
   
}; 
#endif // RECORD_NOT_FOUND_EXCEPTION_H