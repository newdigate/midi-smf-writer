cmake_minimum_required(VERSION 3.10)
project(basic)
if (NOT DEFINED BUILD_FOR_LINUX)
    teensy_include_directories(../../src)
    teensy_add_executable(basic midiread.cpp)
    teensy_target_link_libraries(basic midi_smf_writer SD SdFat SPI cores)
    target_link_libraries(basic.elf stdc++)
else ()
    include_directories(../../src)
    add_executable(midi_smf_writer_basic midiread.cpp)
    target_link_libraries(midi_smf_writer_basic midi_smf_writer teensy_x86_sd_stubs teensy_x86_stubs)
endif ()