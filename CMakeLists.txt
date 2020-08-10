cmake_minimum_required(VERSION 3.15)
project(Cherry)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE DEBUG)

include_directories(include)

add_subdirectory(base)
add_subdirectory(vm)
add_subdirectory(net)
add_subdirectory(conc)

# 指定源文件目录
aux_source_directory(. DIR_SRCS)

add_executable(Cherry ${DIR_SRCS})
target_link_libraries(Cherry base vm net conc)
