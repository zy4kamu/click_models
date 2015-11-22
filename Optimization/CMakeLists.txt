set(TARGET_NAME optimization)

SET(CMAKE_EXE_LINKER_FLAGS "-pthread")

if (${CMAKE_BUILD_TYPE} MATCHES DEBUG)
    MESSAGE("Building DEBUG version: -O0 is used")
    add_compile_options(-O0 -m64 -pthread -std=c++0x -g)
else()
    MESSAGE("Building RELEASE version: -O3 is used")
    add_compile_options(-Ofast -m64 -pthread -std=c++0x)
endif()

file(GLOB sources
    ${CMAKE_CURRENT_SOURCE_DIR}/Clib/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Optimizer/*.cpp
    *.cpp)

file(GLOB headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Clib/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/EventHandler/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Optimizer/*.h
    *.h)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Clib
    ${CMAKE_CURRENT_SOURCE_DIR}/EventHandler
    ${CMAKE_CURRENT_SOURCE_DIR}/Optimizer
    ${CMAKE_CURRENT_SOURCE_DIR}/../Tools
)

add_library(${TARGET_NAME} STATIC ${sources} ${headers})

add_executable(${TARGET_NAME}-app ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${TARGET_NAME}-app tools ${TARGET_NAME} gomp)

ADD_DEFINITIONS("-Wall" "-fPIC")
