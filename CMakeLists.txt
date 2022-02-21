cmake_minimum_required(VERSION 2.6)

project(echo CXX)

set(CMAKE_BUILD_TYPE Debug) 

set(MUDUO_PATH "~/Lib/build/release-install-cpp11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
#  一些Cmake选项
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")


# 一些输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_path(Muduo_INCLUDE_DIR muduo "${MUDUO_PATH}/include")
find_path(Muduo_LIBRARY_DIR libmuduo_net.a "${MUDUO_PATH}/lib")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${Muduo_LIBRARY_DIR})

# 包含头文件
include_directories(${Muduo_INCLUDE_DIR})
# 先找到lib，然后link用
find_library(muduo_base muduo_base)
find_library(muduo_net muduo_net)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

# 可执行文件
add_executable(echo eg1/echo.cc)
# 连接一些乱七八糟的库
# muduo自己的
target_link_libraries(echo ${muduo_net})
target_link_libraries(echo ${muduo_base})
# 其他的
target_link_libraries(echo pthread rt)


# 可执行文件
add_executable(server eg1/server.cc)
# 连接一些乱七八糟的库
# muduo自己的
target_link_libraries(server ${muduo_net})
target_link_libraries(server ${muduo_base})
# 其他的
target_link_libraries(server pthread rt)


find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# 可执行文件
add_executable(client eg1/client.cc)
# 连接一些乱七八糟的库
# muduo自己的
target_link_libraries(client ${muduo_net})
target_link_libraries(client ${muduo_base})
# 其他的
target_link_libraries(client pthread rt)
target_link_libraries(client ${OpenCV_LIBS})


# 可执行文件
add_executable(server_threaded_efficient eg2/server_threaded_efficient.cc)
# 连接一些乱七八糟的库
# muduo自己的
target_link_libraries(server_threaded_efficient ${muduo_net})
target_link_libraries(server_threaded_efficient ${muduo_base})
# 其他的
target_link_libraries(server_threaded_efficient pthread rt)

# 可执行文件
add_executable(server_threaded eg2/server_threaded.cc)
# 连接一些乱七八糟的库
# muduo自己的
target_link_libraries(server_threaded ${muduo_net})
target_link_libraries(server_threaded ${muduo_base})
# 其他的
target_link_libraries(server_threaded pthread rt)


# 可执行文件
add_executable(server_threaded_highperformance eg2/server_threaded_highperformance.cc)
# 连接一些乱七八糟的库
# muduo自己的
target_link_libraries(server_threaded_highperformance ${muduo_net})
target_link_libraries(server_threaded_highperformance ${muduo_base})
# 其他的
target_link_libraries(server_threaded_highperformance pthread rt)


# 可执行文件
add_executable(my_client eg3/my_client.cc)
# 连接一些乱七八糟的库
# muduo自己的
target_link_libraries(my_client ${muduo_net})
target_link_libraries(my_client ${muduo_base})
# 其他的
target_link_libraries(my_client pthread rt)