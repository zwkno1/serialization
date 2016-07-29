cmake_minimum_required (VERSION 3.4)

project(test_xml)

file(COPY test DESTINATION ${PROJECT_BINARY_DIR})  
file(COPY ../../build/codegen DESTINATION ${PROJECT_BINARY_DIR})  
set (GEN_CODE_COMMAND ./codegen -xml test)
execute_process(
	COMMAND ${GEN_CODE_COMMAND}
	RESULT_VARIABLE rv
	)
if(${rv})
	message(SEND_ERROR "Generate code failed.")
endif()


include_directories(.)
include_directories(../../third_party)
include_directories(${PROJECT_BINARY_DIR})

aux_source_directory(. SRC_LIST)
aux_source_directory(../../third_party/binary SRC_LIST)
aux_source_directory(${PROJECT_BINARY_DIR} SRC_LIST)

add_executable(test_xml
	${SRC_LIST}
	)

