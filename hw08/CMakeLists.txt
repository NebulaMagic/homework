cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
# 如果需要指定显卡版本号的话：
set(CMAKE_CUDA_ARCHITECTURES 75)

project(hellocmake LANGUAGES CXX CUDA)

add_executable(main main.cu)
target_include_directories(main PUBLIC include)
