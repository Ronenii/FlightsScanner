cmake_minimum_required(VERSION 3.13)  # CMake version check
project(linux_parent VERSION 1.0)     # Create project
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)              # Enable c++20 standard


# Add main.cpp file of project root directory as source file
set(MAIN_FILE src/main.cpp src/Flight_Service_Funcs.cpp)


#add all src files
file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(FlightService ${MAIN_FILE} ${SRC_FILES})

target_include_directories(FlightService PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/"
        "/usr/local/include/"
        )

