/* 
FileOpenFailedException.h
--------------------------------------------------------------------------------------------------------------------
Rev. 1 - 2024/07/12 Original by Aryan Saadati
--------------------------------------------------------------------------------------------------------------------
This class represents an exception that is thrown when a file fails to open. 
It is derived from the std::logic_error class and allows for custom error messages 
to be provided, which can assist in diagnosing issues related to file opening failures.
*/

#ifndef FILE_OPEN_FAILED_EXCEPTION_H
#define FILE_OPEN_FAILED_EXCEPTION_H

#include <stdexcept>

using std::string;
using std::logic_error;

class FileOpenFailedException : public logic_error {
   
   public:
      // Constructor
      FileOpenFailedException(const string& message = "");
   
}; 
#endif