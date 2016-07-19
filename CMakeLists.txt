
cmake_minimum_required (VERSION 3.4)

project(codegen)

include_directories(.)
include_directories(./third_party/rapidxml)
include_directories(./detail)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

SET(SRC_LIST
	codegen.cpp
	)

add_executable(codegen
	${SRC_LIST}
	)

