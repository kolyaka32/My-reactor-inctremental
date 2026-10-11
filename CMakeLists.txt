# Setting minimum required for cmake versions
cmake_minimum_required(VERSION 3.7.10)

# Setting minimal C++ version for work
set(CMAKE_CXX_STANDARD 14)

# Creating project
project(YARCHE)

# Special for windows
if (WIN32)
    # Hidding console
    set(CMAKE_CXX_FLAGS "-mwindows -static")

    # Setting to link standart libraries statickly
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif (WIN32)

# Add all source files (that needed) under the src directory
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "src/*.rc")
add_executable(${PROJECT_NAME} ${SOURCES})

# Look for side libraries
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(libzip)

# Linking side libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE libzip::zip)
