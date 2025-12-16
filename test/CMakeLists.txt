cmake_minimum_required(VERSION 3.21)

project(control-commonTests LANGUAGES C)

include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(control-common REQUIRED)
  enable_testing()
endif()


add_folders(Test)
