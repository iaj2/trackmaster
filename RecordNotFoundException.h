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