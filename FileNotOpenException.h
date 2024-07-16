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