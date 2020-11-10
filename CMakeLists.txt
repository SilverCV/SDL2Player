cmake_minimum_required(VERSION 3.15)
project(SDL2Play)
include_directories("C:/mingw64/include")
set(CMAKE_CXX_STANDARD 14)
link_directories("C:/mingw64/lib")

add_executable(SDL2Play main.cpp)
target_link_libraries(SDL2Play SDL2)