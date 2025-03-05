cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 REQUIRED COMPONENTS Core Sql)
find_package(redis++ REQUIRED)

add_executable(MyProject main.cpp)

target_link_libraries(MyProject Qt5::Core Qt5::Sql redis++)
include_directories(${REDIS++_INCLUDE_DIRS})
