
cmake_minimum_required (VERSION 3.4)

project(Test)

include_directories(.)
include_directories(../../third_party)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(. SRC_LIST)

add_executable(test_json
	${SRC_LIST}
	)

