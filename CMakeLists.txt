cmake_minimum_required(VERSION 3.22)
project(p2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_FLAGS "-Wall -Wextra -O3 -fPIC")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "-L/usr/lib/llvm-11/lib -lLLVM-11")

add_executable(p2
    src/main.c)
