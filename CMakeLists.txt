cmake_minimum_required(VERSION 3.10)

project(joytime-input-hidapi)

include(GenerateExportHeader)

add_subdirectory(deps/joytime-core)

add_library(joytime-input-hidapi SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-input-hidapi.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/hidapi-handle.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-input-hidapi-wrapper.cpp")
add_library(joytime-input-hidapi_static STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-input-hidapi.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/hidapi-handle.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-input-hidapi-wrapper.cpp")

# <HIDAPI>
string(REGEX MATCH "Linux" LINUX ${CMAKE_SYSTEM_NAME})
string(REGEX MATCH "BSD" BSD ${CMAKE_SYSTEM_NAME})

set(HIDAPI_HID_C_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/hidapi")

if (LINUX)
  if (DEFINED LIBUSB)
    set(HIDAPI_HID_C_PATH "${HIDAPI_HID_C_PATH}/libusb/hid.c")
  else (DEFINED LIBUSB)
    set(HIDAPI_HID_C_PATH "${HIDAPI_HID_C_PATH}/linux/hid.c")
  endif (DEFINED LIBUSB)
elseif (BSD)
  set(HIDAPI_HID_C_PATH "${HIDAPI_HID_C_PATH}/libusb/hid.c")
elseif (WIN32)
  set(HIDAPI_HID_C_PATH "${HIDAPI_HID_C_PATH}/windows/hid.c")
elseif (APPLE)
  set(HIDAPI_HID_C_PATH "${HIDAPI_HID_C_PATH}/mac/hid.c")
else (LINUX)
  message(FATAL_ERROR "Unsupported system for HIDAPI")
endif (LINUX)

add_library(hidapi SHARED "${HIDAPI_HID_C_PATH}")
add_library(hidapi_static STATIC "${HIDAPI_HID_C_PATH}")
# </HIDAPI>

add_dependencies(joytime-input-hidapi joytime-core hidapi)
add_dependencies(joytime-input-hidapi_static hidapi_static)

# <HIDAPI>
#set_target_properties(hidapi PROPERTIES
  #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  #BUILD_TYPE Release
#)
#set_target_properties(hidapi_static PROPERTIES
  #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  #BUILD_TYPE Release
#)
#</HIDAPI>
set_target_properties(joytime-input-hidapi PROPERTIES
  #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  CXX_VISIBILITY_PRESET hidden
  #BUILD_TYPE Release
)
set_target_properties(joytime-input-hidapi_static PROPERTIES
  #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  CXX_VISIBILITY_PRESET hidden
  #BUILD_TYPE Release
)

GENERATE_EXPORT_HEADER(joytime-input-hidapi
  BASE_NAME joytime_input_hidapi
  EXPORT_MACRO_NAME JOYTIME_INPUT_HIDAPI_EXPORT
  EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/include/joytime_input_hidapi_EXPORTS.h"
  STATIC_DEFINE JOYTIME_INPUT_HIDAPI_BUILT_AS_STATIC
)

add_custom_command(TARGET joytime-input-hidapi PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  $<TARGET_FILE_DIR:joytime-core>
  $<TARGET_FILE_DIR:joytime-input-hidapi>)

target_compile_definitions(joytime-input-hidapi_static PUBLIC JOYTIME_INPUT_HIDAPI_BUILT_AS_STATIC=1)

# <HIDAPI>
target_include_directories(hidapi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/hidapi/hidapi")
target_include_directories(hidapi_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/hidapi/hidapi")
# </HIDAPI>
target_include_directories(joytime-input-hidapi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(joytime-input-hidapi_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (WIN32)
  target_link_libraries(hidapi SetupAPI)
  target_link_libraries(hidapi_static SetupAPI)
endif (WIN32)
target_link_libraries(joytime-input-hidapi PUBLIC joytime-core PUBLIC hidapi)
target_link_libraries(joytime-input-hidapi_static PUBLIC joytime-core_static PUBLIC hidapi_static)
