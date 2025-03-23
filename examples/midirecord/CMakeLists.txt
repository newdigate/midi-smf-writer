cmake_minimum_required(VERSION 3.10)
project(midi_smf_writer_example_midi_record)
if (NOT DEFINED BUILD_FOR_LINUX)
    teensy_include_directories(../../src)
    teensy_add_executable(midi_smf_writer_example_midi_record midirecord.cpp)
    teensy_target_link_libraries(midi_smf_writer_example_midi_record midi_smf_writer SD SdFat SPI cores)
    target_link_libraries(midi_smf_writer_example_midi_record.elf stdc++)
else ()
    include_directories(../../src)


    add_executable(midi_smf_writer_example_midi_record midirecord.cpp)
    target_link_libraries(midi_smf_writer_example_midi_record rtmidi teensy_x86_midi_stubs midi_smf_writer teensy_x86_sd_stubs teensy_x86_stubs)
endif ()