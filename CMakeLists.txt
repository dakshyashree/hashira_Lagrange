cmake_minimum_required(VERSION 3.16)
project(secret_finder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(secret_finder src/main.cpp)

# nlohmann/json include
target_include_directories(secret_finder PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Boost headers (point to folder that contains "boost/")
target_include_directories(secret_finder PRIVATE "C:/Users/daksh/Downloads/boost_1_89_0")
