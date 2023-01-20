include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(tools
    messagesTools.h
)

target_link_libraries(tools PUBLIC proto)
