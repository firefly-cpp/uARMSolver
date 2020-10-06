cmake_minimum_required(VERSION 3.5)

project (uARMSolver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3 -s")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")

file(GLOB PROJECT_SOURCES "sources/*.cpp")
file(GLOB PROJECT_HEADERS "sources/*.h")

add_executable(uARMSolver ${PROJECT_SOURCES} ${PROJECT_HEADERS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bin/arm.set DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
