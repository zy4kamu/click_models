set(TARGET_NAME csvManager)

if (${CMAKE_BUILD_TYPE} MATCHES DEBUG)
    MESSAGE("Building DEBUG version: -O0 is used")
    add_compile_options(-O0 -m64 -std=c++0x -fopenmp -ggdb3)
else()
    MESSAGE("Building RELEASE version: -Ofast is used")
    add_compile_options(-Ofast -m64 -std=c++0x -fopenmp)
endif()


file(GLOB sources *.cpp)
file(GLOB sourceheaders *.hpp)
file(GLOB headers *.h)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../Tools
)

add_library(${TARGET_NAME} STATIC ${sources} ${sourceheaders} ${headers})
ADD_DEFINITIONS("-Wall" "-fPIC")
