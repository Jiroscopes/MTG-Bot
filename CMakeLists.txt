cmake_minimum_required (VERSION 3.6)
project(mtgbot)

add_executable(mtgbot src/main.cpp)

target_include_directories(mtgbot PUBLIC include)

add_subdirectory(deps/sleepy-discord)
target_link_libraries(mtgbot sleepy-discord)
