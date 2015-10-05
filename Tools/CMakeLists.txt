set(TARGET_NAME tools)

if (${CMAKE_BUILD_TYPE} MATCHES DEBUG)
    MESSAGE("Building DEBUG version: -O0 is used")
    add_compile_options(-O0 -m64 -std=c++0x -fopenmp -ggdb3)
else()
    MESSAGE("Building RELEASE version: -Ofast is used")
    add_compile_options(-Ofast -m64 -std=c++0x -fopenmp)
endif()

file(GLOB sources
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IO/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Locale/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OS/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Strings/*.cpp
        *.cpp)
file(GLOB sourceheaders
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IO/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Locale/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OS/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Strings/*.hpp
        *.hpp)
file(GLOB headers
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/IO/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Locale/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/OS/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Strings/*.h
        *.h)

set(CMAKE_INCLUDE_SYSTEM_FLAG_${lang} "-isystem ")

include_directories(
    SYSTEM ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex,
    ${CMAKE_CURRENT_LIST_DIR}
)

add_library(${TARGET_NAME} STATIC ${sources} ${sourceheaders} ${headers})
ADD_DEFINITIONS("-Wall" "-fPIC")
ADD_DEFINITIONS(-DTOOLS_OS_SCRIPT_FOLDER=\"${PROJECT_ROOT_DIR}/Tools/OS/Scripts/\")



#include_directories(
#    SYSTEM ${PROJECT_ROOT_DIR}/utils/RegexpTools/boost_regex
#    ${tools_SOURCE_DIR}/
#)

#add_library(${TARGET_NAME} ${LIBRARY_TYPE} ${tools_SOURCES} ${tools_HEADERS})
#set_target_properties(${TARGET_NAME} PROPERTIES DEFINE_SYMBOL tools_EXPORTS)
#add_executable(${TARGET_NAME}-app ${tools_SOURCE_DIR}/main.cpp)
#target_link_libraries(${TARGET_NAME}-app ${TARGET_NAME})
#ADD_DEFINITIONS("-Wall" "-fPIC")
