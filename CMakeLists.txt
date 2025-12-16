cmake_minimum_required(VERSION 3.21)

include(cmake/prelude.cmake)

project(
    common-control
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES C
)

include(cmake/variables.cmake)

# Build as shared library by default
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")

# ---- Declare library ----

add_library(
    common-control_common-control
    source/common-control.c
)
add_library(common-control::common-control ALIAS common-control_common-control)

include(GenerateExportHeader)
generate_export_header(
    common-control_common-control
    BASE_NAME common-control
    EXPORT_FILE_NAME export/common-control/common-control_export.h
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(common-control_common-control PUBLIC COMMON_CONTROL_STATIC_DEFINE)
endif()

set_target_properties(
    common-control_common-control PROPERTIES
    C_VISIBILITY_PRESET default
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME common-control
    OUTPUT_NAME common-control
)

target_include_directories(
    common-control_common-control ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    common-control_common-control SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(common-control_common-control PUBLIC c_std_99)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT common-control_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of common-control"
  )
endif()

include(cmake/dev-mode.cmake)
