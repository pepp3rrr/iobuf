cmake_minimum_required(VERSION 3.21)

project(iobufTests LANGUAGES C)

include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(iobuf REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(test-format source/test-format.c)
target_link_libraries(test-format PRIVATE iobuf::iobuf)
target_compile_features(test-format PRIVATE c_std_99)

add_test(NAME test-format COMMAND test-format)

add_executable(test-rand source/test-rand.c)
target_link_libraries(test-rand PRIVATE iobuf::iobuf)
target_compile_features(test-rand PRIVATE c_std_99)

add_test(NAME test-rand COMMAND test-rand)

# ---- End-of-file commands ----

add_folders(Test)
