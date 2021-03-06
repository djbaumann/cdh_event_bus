cmake_minimum_required (VERSION 2.8.12.2)

# Change project name to desired name
set (PROJECT_NAME "octopOS")

project (${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# include directory for headers
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

# Change name of executable to desired name
add_executable(${PROJECT_NAME} ${SOURCES})

#link boost
FIND_PACKAGE( Boost 1.55 COMPONENTS program_options REQUIRED system serialization)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# Check if pthreads.h is being used
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(PUBLIC ${PROJECT_NAME} "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}" ${Boost_LIBRARIES})
endif()
