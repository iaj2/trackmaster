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