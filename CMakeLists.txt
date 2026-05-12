cmake_minimum_required(VERSION 3.10)
project(boost)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===================== Boost设置 =====================
set(BOOST_ROOT "D:/SUPT/boost019100")

# 包含头文件
include_directories(${BOOST_ROOT})
link_libraries(ws2_32)

# 主程序
add_subdirectory(test)
add_subdirectory(end_point)
add_subdirectory(sync_client)
add_subdirectory(sync_server)