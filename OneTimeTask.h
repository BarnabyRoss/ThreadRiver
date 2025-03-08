
#ifndef __ONETIMETASK_H__
#define __ONETIMETASK_H__

#include <iostream>
#include "Task.h"


class OneTimeTask : public Task {

public:
  OneTimeTask(TaskFunction func, std::uint64_t id, std::uint8_t priority) : Task(func, id, priority){}
  ~OneTimeTask() = default;
  
};


#endif

