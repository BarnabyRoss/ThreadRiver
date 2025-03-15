#include <iostream>
#include <memory>
#include "ThreadPool.h"

std::mutex mtx;

void executeThread(std::shared_ptr<Scheduler> sched){

  std::lock_guard lock(mtx);
  std::optional<std::shared_ptr<Task>> opt = sched->pop();
  if( opt.has_value() ){

    opt.value()->execute();
  }
}


int main() {

 ThreadPool pool(4);

 /*auto task4 = [](int x){ return x; };
 std::future<int> fut = pool.submit(task4, 1, 10);
 int result = fut.get();
 std::cout << "result = " << result << std::endl;

 std::cout << pool.getTaskQueueSize() << std::endl;*/

 auto future = pool.submit([](int x){ return x * 2; }, Task::PRIORITY_NORMAL, 42);
 int result = future.get();

 std::cout << "result = " << result << std::endl;

// 提交普通优先级任务
//auto normalPriorityFuture = pool.submit([](){ std::cout << "Normal priority task" << std::endl; }, Task::PRIORITY_NORMAL);

 // 提交高优先级任务
//auto highPriorityFuture = pool.submit([](){ std::cout << "High priority task" << std::endl; }, Task::PRIORITY_HIGH);

 // 延迟2秒执行任务
//auto delayedFuture = pool.submit([](){ std::cout << "Delayed task" << std::endl; }, Task::PRIORITY_NORMAL, std::chrono::seconds(2));

try {
  auto future = pool.submit([]() {
      throw std::runtime_error("Task failed");
  }, Task::PRIORITY_NORMAL);
  
  future.get();
} catch (const std::exception& e) {
  std::cout << "Error: " << e.what() << std::endl;
}



  return 0;
}