
cmake_minimum_required(VERSION 3.20)

project(hellocmake VERSION 3.1.4 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS ON)

# （可选）使用 ccache 加速编译
find_program(CCACHE_PROGRAM ccache)


add_subdirectory(legacy)
add_subdirectory(mylib)

file(GLOB main_sources CONFIGURE_DEPENDS src/*.cpp)
add_executable(main ${main_sources})
target_link_libraries(main PRIVATE mylib)
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR} 
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}
)

set(VERSION "\"3.1.4\"" CACHE STRING "VERSION")
target_compile_definitions(main PRIVATE HELLOCMAKE_VERSION=${VERSION})

# （可选）添加 run 作为伪目标方便命令行调用
add_custom_target(run COMMAND $<TARGET_FILE:main>)
