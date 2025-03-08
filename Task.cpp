
#include "Task.h"


bool Task::isReadyToExecute() const{

  return std::chrono::system_clock::now() >= getNextExecuteTime();
}