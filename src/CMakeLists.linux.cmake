cmake_minimum_required(VERSION 3.5)
set(midi_smf_writer_VERSION 1.0.0)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/midi_smf_writer/)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/midi_smf_writer )

find_package(teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

find_package(teensy_x86_sd_stubs)
include_directories(${teensy_x86_sd_stubs_INCLUDE_DIR})

add_library(midi_smf_writer STATIC ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(midi_smf_writer PROPERTIES PUBLIC_HEADER ${HEADER_FILES})

include(CMakePackageConfigHelpers)
configure_package_config_file(../cmake/midi_smf_writer.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/midi_smf_writerConfig.cmake
        INSTALL_DESTINATION ${LIB_INSTALL_DIR}/midi_smf_writer/cmake
        PATH_VARS)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/midi_smf_writerConfigVersion.cmake
        VERSION ${midi_smf_writer_VERSION}
        COMPATIBILITY SameMajorVersion )

install(TARGETS midi_smf_writer DESTINATION
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION "include/midi_smf_writer"
        )

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/midi_smf_writerConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/midi_smf_writerConfigVersion.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/uninstall.cmake
        DESTINATION "lib/cmake/midi_smf_writer" )
##
# copied from: https://gist.github.com/royvandam/3033428
# Add uninstall target
# Requirements: Copy the uninstall.cmake file to the appropriate CMAKE_MODULE_PATH.
#
set(CMAKE_VERBOSE_MAKEFILE 1)
add_custom_target(installarduino
        "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/install_arduino_library.cmake")

add_custom_target(uninstall
        "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/uninstall.cmake")
