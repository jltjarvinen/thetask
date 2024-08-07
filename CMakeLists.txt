cmake_minimum_required(VERSION 3.10)
project(TheTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NUMBER_OF_PORTS 3)
add_definitions(-DNUMBER_OF_PORTS=${NUMBER_OF_PORTS})

add_subdirectory(client)
add_subdirectory(client1)
add_subdirectory(client2)
