project(weather)

add_subdirectory(jsoncpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include)
set(src WinHttpClient.h WinHttpClient.cpp main.cpp)

add_executable(weather ${src})
target_link_libraries(weather jsoncpp)
