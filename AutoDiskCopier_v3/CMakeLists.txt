cmake_minimum_required(VERSION 3.10)
project(AutoDiskCopier)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE INC src/*.h)
file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE RES src/*.rc)

add_executable(AutoDiskCopier WIN32 ${SRC} ${INC} ${RES})
