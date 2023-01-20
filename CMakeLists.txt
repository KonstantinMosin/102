cmake_minimum_required(VERSION 3.14)

project(length-prefixed LANGUAGES CXX)

add_subdirectory(proto)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
