cmake_minimum_required(VERSION 3.16)
set(CXX, gcc)
project(rnec)

add_executable(rnec rnec.cpp)
# target_link_libraries(rnec pthread)
find_package (Threads REQUIRED)

target_link_libraries(rnec
    ${CMAKE_THREAD_LIBS_INIT}
)
# add_library(rnec rnec.cpp)