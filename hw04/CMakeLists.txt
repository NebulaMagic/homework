cmake_minimum_required(VERSION 3.12)
project(hellocmake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#ADD_COMPILE_OPTIONS(-ffast-math -march=native -openmp:experimental)
#在MSVC提醒需要-openmp:experimental 但怎么都没有用

ADD_COMPILE_OPTIONS(-ffast-math -march=native)
add_executable(main main.cpp)

find_package(OpenMP REQUIRED)
target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX)
