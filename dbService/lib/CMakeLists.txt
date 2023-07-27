file(GLOB_RECURSE  SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

#add_library(my_lib SHARED ${SOURCES})
add_library(lib STATIC ${SOURCES})


target_include_directories(lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")