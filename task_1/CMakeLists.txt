cmake_minimum_required(VERSION 3.10)
project(Determinant CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES "*.cpp" "*.p")

add_executable(DeterminantCalculator ${SOURCES})

target_link_libraries(DeterminantCalculator
    PRIVATE Threads::Threads
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
