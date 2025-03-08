
#ifndef __TASK_H__
#define __TASK_H__

#include <iostream>
#include <ctime>
#include <atomic>
#include "ErrorHandler.h"

class Task{

public:
  using TaskFunction = std::function<void()>;
  using TimePoint = std::chrono::system_clock::time_point;

  static const int PRIORITY_HIGH  = 1;
  static const int PRIORITY_NORMAL = 2;
  static const int PRIORITY_LOW = 3;

protected:
  TaskFunction func_;
  std::uint8_t priority_;
  std::atomic<std::uint64_t> id_{0};
  TimePoint nextExecuteTime_;
  ErrorHandler errorHandler_;

public:
  Task(TaskFunction func, std::uint64_t id, std::uint8_t priority) : func_(std::move(func)), id_(id), priority_(priority){
    nextExecuteTime_ = std::chrono::system_clock::now();
  }
  Task(TaskFunction func, std::uint64_t id, std::uint8_t priority, TimePoint executeTime) : 
                                                func_(std::move(func)), id_(id), priority_(priority), nextExecuteTime_(executeTime){}

   template< typename Duration >
   Task(TaskFunction func, std::uint64_t id, std::uint8_t priority, Duration delay) : func_(std::move(func)), id_(id), priority_(priority) {

    auto nowTime = std::chrono::system_clock::now();
    nextExecuteTime_ = nowTime + delay;
   }
                                                
  virtual ~Task() = default;

  virtual void execute();
  virtual std::uint64_t getId(){ return id_; }
  virtual void setId(std::uint64_t id){ id_.store(id); }
  virtual std::uint8_t getPriority() const { return priority_; }

  TimePoint getNextExecuteTime() const { return nextExecuteTime_; }

  bool isReadyToExecute() const;

  ErrorHandler& getErrorHandler(){ return errorHandler_; }

};


#endif
