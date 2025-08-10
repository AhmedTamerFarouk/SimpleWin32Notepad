cmake_minimum_required(VERSION 3.13)
project(NotepadLite VERSION 1.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#testing
enable_testing()

# build a Windows GUI app not a console app
add_executable(MyNotePad WIN32
    src/notepad.cpp
    src/fileutils.cpp
)

# to be able to include in executable without adding full path 
target_include_directories(MyNotePad PRIVATE ${CMAKE_SOURCE_DIR}/include)

# force Unicode entry point 
target_link_options(MyNotePad PRIVATE -municode)

#add CMakeLists of google tests folder
add_subdirectory(third_party/googletest)

#add CMakelists of tests folder
add_subdirectory(tests)