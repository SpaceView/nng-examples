# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (nng_examples)

#find_package(nng REQUIRED)
#find_path(NNG_INCLUDE_DIRS "nng/nng.h")
#target_link_libraries(${PROJECT_NAME} ${nng_LIBS} )
#if (NNG_FOUND)
#include_directories(${NNG_INCLUDE_DIRS})
#endif (NNG_FOUND)


#add_subdirectory (Hello)
#add_library (bus bus.c)
#target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable (bus bus.c)
#target_include_directories(bus PRIVATE ${NNG_INCLUDE_DIR})
#target_link_libraries(bus PUBLIC ${nng_LIBS} )
add_executable(pair pair.c)
add_executable(pipeline pipeline.c)
add_executable(pubsub pubsub.c)
add_executable(reqprep reqprep.c)
add_executable(survey survey.c)
