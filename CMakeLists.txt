cmake_minimum_required(VERSION 3.5)

project(midi_smf_writer C CXX)

if (NOT DEFINED TEENSY_VERSION)
    set(BUILD_FOR_LINUX 1)
    add_definitions(-DBUILD_FOR_LINUX)
endif()

include(cmake_declare_and_fetch.cmake.in)
IF (DEFINED BUILD_FOR_TEENSY)
    DeclareAndFetchNoSource(teensy_cmake_macros         https://github.com/newdigate/teensy-cmake-macros.git main)
    include(${teensy_cmake_macros_SOURCE_DIR}/CMakeLists.include.txt)
    import_arduino_library(cores ${teensy_cores_SOURCE_DIR}/teensy4 avr debug util)
    import_arduino_library_git(SPI https://github.com/PaulStoffregen/SPI master "")
    import_arduino_library_git(SdFat https://github.com/PaulStoffregen/SdFat.git master "src" common DigitalIO ExFatLib FatLib FsLib iostream SdCard SpiDriver)
    import_arduino_library_git(SD https://github.com/PaulStoffregen/SD.git Juse_Use_SdFat src)
    import_arduino_library_git(MIDI https://github.com/PaulStoffregen/MIDI master src)
    add_subdirectory(src)
    add_subdirectory(examples)
else()
    set(CMAKE_CXX_STANDARD 14)
    DeclareAndFetch(teensy_x86_stubs                https://github.com/newdigate/teensy-x86-stubs.git main src)
    DeclareAndFetch(teensy_x86_sd_stubs             https://github.com/newdigate/teensy-x86-sd-stubs.git main src)
    add_subdirectory(src)
    if(BUILD_EXAMPLES)
        DeclareAndFetch(rtmidi                      https://github.com/thestk/rtmidi.git master "/")
        DeclareAndFetch(teensy_x86_midi_stubs       https://github.com/newdigate/teensy-x86-midi-stubs main src)
            DeclareAndFetchSubLibrary(teensy_x86_midi_stubs     extras/rtmidi/src)
        add_subdirectory(examples)
    endif ()
    if(BUILD_TESTS)
        DeclareAndFetch(midi_smf_reader            https://github.com/newdigate/midi-smf-reader.git main src)
        add_subdirectory(test)
    endif ()
endif()