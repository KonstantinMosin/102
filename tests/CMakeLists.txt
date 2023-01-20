include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(length-prefixed-test
    main.cpp
)

target_include_directories(length-prefixed-test PRIVATE "${CMAKE_SOURCE_DIR}")
target_include_directories(length-prefixed-test PRIVATE "${CMAKE_BINARY_DIR}")

target_link_libraries(length-prefixed-test
    gtest
    gtest_main
    tools
)

include(GoogleTest)
gtest_discover_tests(length-prefixed-test)
