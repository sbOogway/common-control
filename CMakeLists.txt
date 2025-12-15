cmake_minimum_required(VERSION 3.21)

include(cmake/prelude.cmake)

project(
    control-common
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES C
)

include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    control-common_control-common
    source/control-common.c
)
add_library(control-common::control-common ALIAS control-common_control-common)

include(GenerateExportHeader)
generate_export_header(
    control-common_control-common
    BASE_NAME control-common
    EXPORT_FILE_NAME export/control-common/control-common_export.h
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(control-common_control-common PUBLIC CONTROL_COMMON_STATIC_DEFINE)
endif()

set_target_properties(
    control-common_control-common PROPERTIES
    C_VISIBILITY_PRESET hidden
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME control-common
    OUTPUT_NAME control-common
)

target_include_directories(
    control-common_control-common ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    control-common_control-common SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(control-common_control-common PUBLIC c_std_99)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT control-common_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of control-common"
  )
endif()

include(cmake/dev-mode.cmake)
