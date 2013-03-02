cmake_minimum_required(VERSION 2.8)
project(jsoncpp)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(INC_FILES
${CMAKE_CURRENT_SOURCE_DIR}/include/json/autolink.h
${CMAKE_CURRENT_SOURCE_DIR}/include/json/config.h
${CMAKE_CURRENT_SOURCE_DIR}/include/json/features.h
${CMAKE_CURRENT_SOURCE_DIR}/include/json/forwards.h
${CMAKE_CURRENT_SOURCE_DIR}/include/json/json.h
${CMAKE_CURRENT_SOURCE_DIR}/include/json/reader.h
${CMAKE_CURRENT_SOURCE_DIR}/include/json/value.h
${CMAKE_CURRENT_SOURCE_DIR}/include/json/writer.h
)

SET(SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib_json/json_internalarray.inl
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib_json/json_internalmap.inl
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib_json/json_reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib_json/json_value.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib_json/json_valueiterator.inl
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib_json/json_writer.cpp
)

add_library(jsoncpp STATIC ${INC_FILES} ${SRC_FILES})
