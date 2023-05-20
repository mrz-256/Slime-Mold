cmake_minimum_required(VERSION 3.25)
project(MOLDY)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(MOLDY main.cpp AgentProcessor.cpp AgentProcessor.h Agent.h)

target_link_libraries(MOLDY sfml-graphics)