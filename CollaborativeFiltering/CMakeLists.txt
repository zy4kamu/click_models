cmake_minimum_required(VERSION 2.8)
project(collaborative_filtering)

#set(CMAKE_CXX_COMPILER /usr/bin/g++-4.8)
SET(CMAKE_EXE_LINKER_FLAGS "-pthread")

if (${CMAKE_BUILD_TYPE} MATCHES DEBUG)
    MESSAGE("Building DEBUG version: -O0 is used")
    add_compile_options(-O0 -m64 -pthread -std=c++0x -g)
else()
    MESSAGE("Building RELEASE version: -O3 is used")
    add_compile_options(-Ofast -m64 -pthread -std=c++0x)
endif()


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../ClickModels
    ${CMAKE_CURRENT_SOURCE_DIR}/../FileManager
    ${CMAKE_CURRENT_SOURCE_DIR}/../Tools)
link_directories()

file(GLOB sources
        *.cpp)
file(GLOB sourceheaders
        *.hpp)
file(GLOB headers
        *.h)

add_library(collaborative-filtering STATIC ${sources} ${headers} ${sourceheaders})

add_executable(collaborative-filtering-app main.cpp)
target_link_libraries(collaborative-filtering-app LINK_PUBLIC pthread collaborative-filtering tools csvManager click-models)


