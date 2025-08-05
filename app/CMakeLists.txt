cmake_minimum_required(VERSION 3.10)
project(log_app)

add_executable(log_app main.cpp)
add_executable(threaded_enter_app threaded_enter.cpp)

target_link_libraries(log_app log_proj)
target_link_libraries(threaded_enter_app log_proj pthread)