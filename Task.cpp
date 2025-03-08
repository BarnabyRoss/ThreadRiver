
#include "Task.h"


bool Task::isReadyToExecute() const{

  return std::chrono::system_clock::now() >= getNextExecuteTime();
}

void Task::execute(){

  if( func_ ){

    try{
    
      func_();
  
    }catch(const std::exception& e){
  
      errorHandler_.setError(ErrorType::TASK_ERROR, std::string("Task Execute Failed : ") + e.what());
      throw;
    }catch(...){

      errorHandler_.setError(ErrorType::TASK_ERROR, std::string("Task Execute Failed : ") + "id = " + std::to_string(getId()) + std::string(" Unknow Error"));
      throw;
    }
  }
}