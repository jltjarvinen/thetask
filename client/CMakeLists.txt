add_library(clientlib STATIC 
    ${CMAKE_CURRENT_LIST_DIR}/src/client.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/control.cpp)

target_include_directories(clientlib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(clientlib PUBLIC pthread)
