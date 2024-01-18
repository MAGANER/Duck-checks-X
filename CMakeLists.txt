cmake_minimum_required(VERSION 3.10)
project(DCX)
set (CMAKE_CXX_STANDARD 11)

set(SOURCE main.cpp)
set(HEADERS mbytesreader.h strcomp.h Gumbo.hpp htmlreader.h)

add_executable(cmx ${SOURCE} ${HEADERS})
