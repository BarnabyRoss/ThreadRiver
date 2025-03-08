#ifndef __ERROR_HANDLER_H__
#define __ERROR_HANDLER_H__

#include <string>
#include <exception>

enum struct ErrorType{

  NONE = 0,
  TASK_ERROR,
  THREAD_ERROR,
  SYSTEM_ERROR

};

class ErrorHandler{

private:
  ErrorType errorType_;
  std::string errorMessage_;
  bool hasError_;

public:
  ErrorHandler() : errorType_(ErrorType::NONE), errorMessage_(""), hasError_(false){}

  void setError(ErrorType type, const std::string& message);

  void clearError();

  std::string getErrorMessage() const;
  ErrorType getErrorType() const;
  bool hasError() const;

};


#endif