# cmake version
cmake_minimum_required(VERSION 3.10)

# project name
project(http_server)

# Find boost
find_package(Boost REQUIRED)

# Include Boost headers
include_directories(${Boost_INCLUDE_DIRS})

# c++ version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add executable 
add_executable(server ./src/main.cpp ./src/http_server.cpp)
