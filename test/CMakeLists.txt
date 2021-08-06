cmake_minimum_required(VERSION 3.5)
project(tests CXX C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

find_package(teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

find_package(teensy_x86_sd_stubs)
include_directories(${teensy_x86_sd_stubs_INCLUDE_DIR})

find_package(midi_smf_reader)
include_directories(${midi_smf_reader_INCLUDE_DIR})

set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

include_directories(../src)

add_executable(tests
        test_read_midi.cpp)

target_link_libraries(tests midi_smf_writer)
target_link_libraries(tests ${midi_smf_reader_LIBS})
target_link_libraries(tests ${teensy_x86_stubs_LIBS})
target_link_libraries(tests ${teensy_x86_sd_stubs_LIBS})
target_link_libraries(tests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})