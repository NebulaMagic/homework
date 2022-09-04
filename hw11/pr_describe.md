1.

 为什么不指定 cmake_minimum_required 会导致下面在 project 处出错？
#cmake_minimum_required(VERSION 3.10)

```
CMAKE要求顶层CMakeLists必须要有这一句。
```

2.

 如何让构建类型默认为 Release？
#set(CMAKE_BUILD_TYPE Release)

```
将CMAKE_BUILD_TYPE变量设置为Release。
```

3.

 这样设置 C++14 的方式对吗？请改正
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14")

```
不对，不跨平台，要用CMAKE_CXX_STANDARD变量设置
```

4.
   
 这样需要一个个写出所有文件很麻烦，请改成自动批量添加源文件
set(main_sources "src/main.cpp" "src/other.cpp" "src/dummy.cpp" "src/veryusefulfile.cpp")
add_executable(main ${main_sources})

```
这里我用file(GLOB … )。
```
  
5

 请改为项目的正确版本（用变量来获取）
target_compile_definitions(main PRIVATE HELLOCMAKE_VERSION="233.33.33")

```
设置了一个CACHE变量
```

6.

 为什么 legacy 这个只有 .c 文件的对象出错了？
add_executable(legacy "legacy.c")

```
顶层CMakeLists的project只设置的C++的项目，添加个C，或者使用enable_language(C)。（多写一句project(name  C)也可以？）
```

7.

 请改用自动批量查找所有 .cpp 和 .h 文件：
set(mylib_sources "mylib/mylib.cpp" "mylib/mylib.h")

```
这里我用file(GLOB_RECURSE … )。
```


8.

 使用 SHARED 在 Windows 上会遇到什么困难？请尝试修复!
add_library(mylib SHARED ${mylib_sources})
target_compile_definitions(mylib PRIVATE MYLIB_EXPORT)

```
编译后输出的DLL需要另外拷贝到可执行文件同目录或者另外设置环境变量麻烦，用set_target_properties设置输出位置到可执行文件同样的PROJECT_BINARY_DIR地方。
（这里顺便在顶层CMakeLists也改了执行文件的输出位置，否则不知道为什么在vscode下终端输出位置自己会多加一个Debug文件夹）
```



9.

 这里应该用 PRIVATE 还是 PUBLIC？
target_include_directories(mylib PRIVATE .)

```
使用PUBLIC让链接到mylib库的目标也能使用到mylib库添加的头文件目录。
```
