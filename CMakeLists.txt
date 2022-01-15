cmake_minimum_required(VERSION 3.0)
project(filesystem)

include_directories(

)

link_directories(

)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_CXX_FLAGS "")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_executable(fs ${SOURCES})