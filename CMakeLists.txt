cmake_minimum_required(VERSION 3.11)
project(Simulator)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 11)

SET(GCC_COVERAGE_COMPILE_FLAGS "-m64 -fno-rtti -fno-exceptions -Wno-error  -Wwrite-strings -Wno-narrowing -fpermissive")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Define library source and include directories
set(LIBBGI_SRC ${CMAKE_SOURCE_DIR}/src/libbgi_src)
set(LIBBGI_HEADER ${CMAKE_SOURCE_DIR}/include/libbgi_header)

set(PROJECT_SRC ${CMAKE_SOURCE_DIR}/src/project_src)
set(PROJECT_HEADER ${CMAKE_SOURCE_DIR}/include/project_header)

add_library(simulator ${LIBBGI_SRC}/drawing.cxx
        ${LIBBGI_SRC}/misc.cxx
        ${LIBBGI_SRC}/mouse.cxx
        ${LIBBGI_SRC}/palette.cxx
        ${LIBBGI_SRC}/text.cxx
        ${LIBBGI_SRC}/winbgi.cxx
        ${LIBBGI_SRC}/winthread.cxx
        ${LIBBGI_SRC}/dibutil.cxx
        ${LIBBGI_SRC}/file.cxx
        ${LIBBGI_HEADER}/winbgim.h
        ${LIBBGI_HEADER}/winbgitypes.h
        )

target_include_directories(simulator
        PUBLIC ${PROJECT_HEADER}
        PUBLIC ${LIBBGI_HEADER})

target_link_libraries(simulator
        PUBLIC gdiplus comdlg32 gdi32 uuid oleaut32 ole32)

add_executable(project 
${PROJECT_SRC}/main.cpp
${PROJECT_SRC}/mainMenu.cpp
${PROJECT_SRC}/userInput.cpp
${PROJECT_SRC}/projectile.cpp
)

target_link_libraries(project simulator)
