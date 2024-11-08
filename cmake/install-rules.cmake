if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/iobuf-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package iobuf)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT iobuf_Development
)

install(
    TARGETS iobuf_iobuf
    EXPORT iobufTargets
    RUNTIME #
    COMPONENT iobuf_Runtime
    LIBRARY #
    COMPONENT iobuf_Runtime
    NAMELINK_COMPONENT iobuf_Development
    ARCHIVE #
    COMPONENT iobuf_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    iobuf_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE iobuf_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(iobuf_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${iobuf_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT iobuf_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${iobuf_INSTALL_CMAKEDIR}"
    COMPONENT iobuf_Development
)

install(
    EXPORT iobufTargets
    NAMESPACE iobuf::
    DESTINATION "${iobuf_INSTALL_CMAKEDIR}"
    COMPONENT iobuf_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
