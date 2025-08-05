cmake_minimum_required(VERSION 3.10)
project(log_proj)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

add_library(log_proj src/logger.cpp)
target_include_directories(log_proj PUBLIC include)

add_subdirectory(app)