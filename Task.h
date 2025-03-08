
#ifndef __TASK_H__
#define __TASK_H__

#include <iostream>
#include <ctime>
#include <atomic>

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

public:
  Task(TaskFunction func, std::uint64_t id, std::uint8_t priority) : func_(std::move(func)), id_(id), priority_(priority){}
  Task(TaskFunction func, std::uint64_t id, std::uint8_t priority, TimePoint executeTime) : 
                                                func_(std::move(func)), id_(id), priority_(priority), nextExecuteTime_(executeTime){}
  virtual ~Task() = default;

  virtual void execute() { if( func_ ){ func_();}}
  virtual std::uint64_t getId(){ return id_; }
  virtual void setId(std::uint64_t id){ id_.store(id); }
  virtual std::uint8_t getPriority() const { return priority_; }

  TimePoint getNextExecuteTime() const { return nextExecuteTime_; }

  bool isReadyToExecute() const;

};


#endif
