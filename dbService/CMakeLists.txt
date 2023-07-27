cmake_minimum_required(VERSION 3.13)  # CMake version check
project(linux_child VERSION 1.0)     # Create project
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)              # Enable c++20 standard

#add custom lib
add_subdirectory(lib)

# Add main.cpp file of project root directory as source file
set(MAIN_FILE src/main.cpp src/Db_Service_Funcs.cpp)
set(SCRIPT_FILE cmake-build-debug/flight_scanner.sh)

#add all src files
file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(dbService ${MAIN_FILE} ${SRC_FILES})

target_link_libraries(dbService PUBLIC lib)

target_include_directories(dbService PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
        "/usr/local/include/"
        )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_FILE} ${CMAKE_BINARY_DIR}/${SCRIPT_FILE} COPYONLY)
