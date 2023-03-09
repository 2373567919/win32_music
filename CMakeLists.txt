cmake_minimum_required(VERSION 3.24)
project(music C)

set(CMAKE_C_STANDARD 23)
include_directories(${PROJECT_SOURCE_DIR}/bass/include)
link_directories(${PROJECT_SOURCE_DIR}/bass/lib)
link_libraries(bass bassflac)
add_executable(music WIN32 main.c musicInfo.c music.c)
add_custom_command(TARGET music POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/bass/bin" $<TARGET_FILE_DIR:music>)

