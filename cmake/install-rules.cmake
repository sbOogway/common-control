if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/common-control"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package common-control)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT common-control_Development
)

install(
    TARGETS common-control_common-control
    EXPORT common-controlTargets
    RUNTIME #
    COMPONENT common-control_Runtime
    LIBRARY #
    COMPONENT common-control_Runtime
    NAMELINK_COMPONENT common-control_Development
    ARCHIVE #
    COMPONENT common-control_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    common-control_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE common-control_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(common-control_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${common-control_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT common-control_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${common-control_INSTALL_CMAKEDIR}"
    COMPONENT common-control_Development
)

install(
    EXPORT common-controlTargets
    NAMESPACE common-control::
    DESTINATION "${common-control_INSTALL_CMAKEDIR}"
    COMPONENT common-control_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
