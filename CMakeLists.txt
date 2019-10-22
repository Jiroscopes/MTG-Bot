cmake_minimum_required (VERSION 3.6)
project(mtgbot)

add_executable(mtgbot main.cpp)

add_subdirectory(deps/sleepy-discord)
target_link_libraries(mtgbot sleepy-discord)
