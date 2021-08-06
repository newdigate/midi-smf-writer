cmake_minimum_required(VERSION 3.10)
project(midi_smf_writer)

set(SOURCE_FILES
        smfwriter.cpp)

set(HEADER_FILES
        smfwriter.h)

if (DEFINED BUILD_FOR_LINUX)    
        include(CMakeLists.linux.cmake)
else()
        include(CMakeLists.teensy.cmake)
endif()
