cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

project(main LANGUAGES CXX)

add_executable(main main.cpp)

#find_package(OpenMP REQUIRED)
#target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX)

# find_package(TBB REQUIRED)
# target_link_libraries(main PUBLIC TBB::tbb)

#find_package(benchmark REQUIRED)
#target_link_libraries(main PUBLIC benchmark::benchmark)

target_compile_definitions(main PRIVATE NOMINMAX)
target_include_directories(main PRIVATE TBB_msvc_build_2020U1/include)
target_link_directories(main  PRIVATE TBB_msvc_build_2020U1/Release)