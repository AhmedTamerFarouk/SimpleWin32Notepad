# Read/Write File tests
add_executable(test_file fileReadWrite_tests.cpp
                         ../src/fileutils.cpp)

target_link_libraries(test_file PRIVATE gtest_main)
target_include_directories(test_file PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME FileTests COMMAND test_file)
