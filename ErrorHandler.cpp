
#include "ErrorHandler.h"

void ErrorHandler::setError(ErrorType type, const std::string& message){

  errorType_ = type;
  errorMessage_ = message;
  hasError_ = true;
}

void ErrorHandler::clearError(){

  errorType_ = ErrorType::NONE;
  errorMessage_ = "";
  hasError_ = false;
}

std::string ErrorHandler::getErrorMessage() const{

  return errorMessage_;
}

ErrorType ErrorHandler::getErrorType() const{

  return errorType_;
}

bool ErrorHandler::hasError() const{

  return hasError_;
}

