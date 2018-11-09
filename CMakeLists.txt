cmake_minimum_required(VERSION 3.12.0)
project(boost_graph_example)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS
             graph
             regex
             REQUIRED)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(boost_graph_example main.cpp)
target_compile_options(boost_graph_example PUBLIC -Wall -pedantic)
target_link_libraries(boost_graph_example
  ${Boost_LIBRARIES}
  ${CMAKE_DL_LIBS} # Required for Boost_DLL
  pthread)
