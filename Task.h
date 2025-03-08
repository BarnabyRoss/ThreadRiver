
#ifndef __TASK_H__
#define __TASK_H__

#include <iostream>
#include <ctime>
#include <atomic>

class Task{

public:
  using TaskFunction = std::function<void()>;

  static const int PRIORITY_HIGH  = 1;
  static const int PRIORITY_NORMAL = 2;
  static const int PRIORITY_LOW = 3;
  
protected:
  TaskFunction func_;
  std::atomic<std::uint64_t> id_;
  std::uint8_t priority_;

public:
  Task(TaskFunction func, std::uint64_t id, std::uint8_t priority) : func_(std::move(func)), id_(id), priority_(priority){}
  virtual ~Task() = default;

  virtual void execute() { if( func_ ){ func_();}}
  virtual std::uint64_t getId() const{ return id_; }
  virtual std::uint8_t getPriority() const { return priority_; }

};

#endif
