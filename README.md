# ThreadRiver

一个基于 C++17 的高性能线程池实现，支持任务优先级、延迟执行和异常处理机制。

## 特性

- 支持任务优先级调度（高、中、低三级优先级）
- 支持异步任务提交和结果获取
- 支持延迟执行任务
- 完善的异常处理机制
- 线程数量动态调整
- 基于优先级队列的任务调度
- 线程安全
- 提供线程监控接口（获取线程数量，任务队列长度，已完成任务数等）


## 依赖

- C++17 或更高版本
- CMake 3.10 或更高版本
- GTest (用于单元测试)

## 构建和安装

```bash
mkdir build
cd build
cmake ..
make

构建和安装

基本使用
#include "ThreadPool.h"

int main() {
    // 创建一个具有2个线程的线程池
    ThreadPool pool(2);

    // 提交一个简单任务
    auto future = pool.submit([](int x) { 
        return x * 2; 
    }, Task::PRIORITY_NORMAL, 42);

    // 获取结果
    int result = future.get();  // result = 84
}

优先级任务

提交高优先级任务
auto highPriorityFuture = pool.submit([]() { std::cout << "High priority task"; }, Task::PRIORITY_HIGH);

提交普通优先级任务
auto normalPriorityFuture = pool.submit([](){ std::cout << "Normal priority task"; }, Task::PRIORITY_NORMAL);

延迟执行
// 延迟2秒执行任务
auto delayedFuture = pool.submit([](){ 
    std::cout << "Delayed task
"; 
}, Task::PRIORITY_NORMAL, std::chrono::seconds(2));

异常处理
try {
    auto future = pool.submit([]() {
        throw std::runtime_error("Task failed");
    }, Task::PRIORITY_NORMAL);
    
    future.get();
} catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
}

API文档
构造函数
ThreadPool(size_t threadCount);

提交任务
template<typename F, typename... Args>
auto submit(F&& func, uint8_t priority, Args&&... args);

调整线程数量
void resize(size_t threadCount);

