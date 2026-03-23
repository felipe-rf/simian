cmake_minimum_required(VERSION 3.21)
project(simian)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow CONFIG REQUIRED)
find_package(PostgreSQL REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(simian ${SOURCES})

target_include_directories(simian PRIVATE src)

target_link_libraries(simian PRIVATE Crow::Crow PostgreSQL::PostgreSQL)