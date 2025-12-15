cmake_minimum_required(VERSION 3.21)

project(control-commonTests LANGUAGES C)

include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(control-common REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(control-common_test source/control-common_test.c)
target_link_libraries(control-common_test PRIVATE control-common::control-common)
target_compile_features(control-common_test PRIVATE c_std_99)

add_test(NAME control-common_test COMMAND control-common_test)

# ---- End-of-file commands ----

add_folders(Test)
