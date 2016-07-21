
cmake_minimum_required (VERSION 3.4)

project(test_xml)

include_directories(.)
include_directories(../../../third_party)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

SET(SRC_LIST
	test.cpp
	test_xml.cpp
	)

add_executable(test_xml
	${SRC_LIST}
	)

