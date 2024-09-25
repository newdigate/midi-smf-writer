cmake_minimum_required(VERSION 3.5)
set(midi_smf_writer_VERSION 1.0.0)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/midi_smf_writer/)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/midi_smf_writer )

find_library(teensy_x86_stubs NAME teensy_x86_stubs)

find_library(teensy_x86_sd_stubs NAME teensy_x86_sd_stubs)

add_library(midi_smf_writer STATIC ${HEADER_FILES} ${SOURCE_FILES})
