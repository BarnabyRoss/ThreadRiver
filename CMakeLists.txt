# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(ThreadRiver)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置构建类型（如果没有指定）
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# 指定头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 获取库源文件 （排除main.cpp)
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# 创建线程池库
add_library(threadriver STATIC ${SOURCES})
target_include_directories(threadriver PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 连接必要的库 （如果有需要，比如pthread)
target_link_libraries(threadriver PRIVATE pthread)

# 创建可执行文件
add_executable(threadriver_app ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(threadriver_app PRIVATE threadriver)

# 安装规则
install(TARGETS threadriver
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/threadriver)

# 打印配置信息
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ 标准: ${CMAKE_CXX_STANDARD}")
