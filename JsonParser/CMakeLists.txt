cmake_minimum_required(VERSION 3.10...3.27)

project(JsonParser VERSION 1.0 DESCRIPTION "An exercise." LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
add_subdirectory(src)
