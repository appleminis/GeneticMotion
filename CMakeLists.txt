project(geneticmotion)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

set(LIBS)

add_definitions(-std=c++11)
add_definitions(-D_USE_MATH_DEFINES)    # Use math definitions (M_PI, etc...)
add_definitions(-O3 -W -Wall -pedantic -Wpointer-arith -Wwrite-strings -Wno-long-long$(THREADSCXXFLAGS))  # Display all warnings, mark warnings as error

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
    add_definitions(-O0)
else()
    add_definitions(-O3)
    add_definitions(-D__NO_INLINE__) # Fix "_hypot" issue with math.h/cmath and -O3 optimization level
endif()

set(LIBS ${LIBS} "-lGL -lGLU -lglut")

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIBS})
