cmake_minimum_required(VERSION 3.21)

include(cmake/prelude.cmake)

project(
    iobuf
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES C
)

include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    iobuf_iobuf
    source/iobuf.c
)
add_library(iobuf::iobuf ALIAS iobuf_iobuf)

include(GenerateExportHeader)
generate_export_header(
    iobuf_iobuf
    BASE_NAME iobuf
    EXPORT_FILE_NAME export/iobuf/iobuf_export.h
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(iobuf_iobuf PUBLIC IOBUF_STATIC_DEFINE)
endif()

set_target_properties(
    iobuf_iobuf PROPERTIES
    C_VISIBILITY_PRESET hidden
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME iobuf
    OUTPUT_NAME iobuf
)

target_include_directories(
    iobuf_iobuf ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    iobuf_iobuf SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(iobuf_iobuf PUBLIC c_std_99)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${iobuf_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT iobuf_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of iobuf"
  )
endif()

include(cmake/dev-mode.cmake)
