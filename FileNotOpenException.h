/* 
FileNotOpenException.h
--------------------------------------------------------------------------------------------------------------------
Rev. 1 - 2024/07/12 Original by Aryan Saadati
--------------------------------------------------------------------------------------------------------------------
This class represents an exception that is thrown when a file operation is attempted 
on a file that is not open. It is derived from the std::logic_error class and allows for 
custom error messages to be passed, which can help in debugging and error handling.

*/

#ifndef FILE_NOT_OPEN_EXCEPTION_H
#define FILE_NOT_OPEN_EXCEPTION_H

#include <stdexcept>

using std::string;
using std::logic_error;

class FileNotOpenException : public logic_error {
   
   public:
      // Constructor
      FileNotOpenException(const string& message = "");
   
}; 
#endif