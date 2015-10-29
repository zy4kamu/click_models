cmake_minimum_required(VERSION 2.8)
project(yandex_cpp)

#set(CMAKE_CXX_COMPILER /usr/bin/g++-4.8)
SET(CMAKE_EXE_LINKER_FLAGS "-pthread")

if (${CMAKE_BUILD_TYPE} MATCHES DEBUG)
    MESSAGE("Building DEBUG version: -O0 is used")
    add_compile_options(-O0 -m64 -pthread -std=c++0x -g)
else()
    MESSAGE("Building RELEASE version: -O3 is used")
    add_compile_options(-Ofast -m64 -std=c++11 -pthread)
endif()


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../FileManager
    ${CMAKE_CURRENT_SOURCE_DIR}/../Tools)
link_directories()

file(GLOB sources
        ${CMAKE_CURRENT_SOURCE_DIR}/strategies/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/embeddings/*.cpp
        *.cpp)
file(GLOB sourceheaders
        ${CMAKE_CURRENT_SOURCE_DIR}/strategies/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/embeddings/*.hpp
        *.hpp)
file(GLOB headers
        ${CMAKE_CURRENT_SOURCE_DIR}/strategies/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/embeddings/*.h
        *.h)


add_library(click-models STATIC ${sources} ${headers} ${sourceheaders})

add_executable(click-models-app main.cpp)
target_link_libraries(click-models-app LINK_PUBLIC pthread click-models tools csvManager)

add_executable(embeddings-app embeddings/main.cpp)
target_link_libraries(embeddings-app LINK_PUBLIC pthread click-models tools csvManager)


