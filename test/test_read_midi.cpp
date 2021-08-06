#define BOOST_TEST_MODULE BasicArduinoTests
#include <boost/test/unit_test.hpp>
#include "default_test_fixture.h"
#include "smfwriter.h"
#include "midireader.h"
BOOST_AUTO_TEST_SUITE(basic_midi_write_test)

    extern unsigned int midi_monophonic_len;
    extern unsigned char midi_monophonic_mid[];


    unsigned int get_microseconds_per_tick(double beats_per_minute) {
        double micros_per_beat = 60000000.0 / beats_per_minute;
        unsigned int micros_per_tick = micros_per_beat / 480;
        return micros_per_tick;
    }

    BOOST_FIXTURE_TEST_CASE(can_write_basic_note, DefaultTestFixture) {

        SD.setSDCardFolderPath("output");

        system("echo ${PWD}");

        SmfWriter writer;

        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addEvent(0, 0x90, 54, 127, 0);
        writer.flush();

        midireader reader;
        reader.open(actualFileName);

        double microsPerTick = get_microseconds_per_tick(120.0);

        int totalNumNotesRead = 0;
        for (int t = 0; t < reader.getNumTracks(); t++)
        {
            reader.setTrackNumber(t);
            midimessage midiMessage {};
            int i = 0;
            long totalTicks = 0;
            long microseconds = 0;
            while (reader.read(midiMessage)) {
                totalTicks += midiMessage.delta_ticks;
                microseconds += microsPerTick * midiMessage.delta_ticks;
                if (midiMessage.isTempoChange) {
                    printf("tempo change: %f\n", midiMessage.tempo);
                } else 
                printf("%5d: [%2d,%4d]: %6d: delta: %3d\tstatus: 0x%2x\tkey: %3d\tvelocity: %3d\tchannel: %2d\t\n",
                       microseconds/1000,
                       t,
                       i,
                       totalTicks,
                       midiMessage.delta_ticks,
                       midiMessage.status,
                       midiMessage.key,
                       midiMessage.velocity,
                       midiMessage.channel);
                i++;
            }
            totalNumNotesRead += i;
        }

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(totalNumNotesRead, 1);
    }

    BOOST_FIXTURE_TEST_CASE(can_write_delayed_note, DefaultTestFixture) {

        SD.setSDCardFolderPath("output");

        system("echo ${PWD}");

        SmfWriter writer;
        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addEvent(1000, 0x90, 54, 127, 0);
        writer.flush();

        midireader reader;
        reader.open(actualFileName);

        double microsPerTick = get_microseconds_per_tick(120.0);

        int totalNumNotesRead = 0;
        for (int t = 0; t < reader.getNumTracks(); t++)
        {
            reader.setTrackNumber(t);
            midimessage midiMessage {};
            int i = 0;
            long totalTicks = 0;
            long microseconds = 0;
            while (reader.read(midiMessage)) {
                totalTicks += midiMessage.delta_ticks;
                microseconds += microsPerTick * midiMessage.delta_ticks;
                if (midiMessage.isTempoChange) {
                    printf("tempo change: %f\n", midiMessage.tempo);
                } else 
                printf("%5d: [%2d,%4d]: %6d: delta: %3d\tstatus: 0x%2x\tkey: %3d\tvelocity: %3d\tchannel: %2d\t\n",
                       microseconds/1000,
                       t,
                       i,
                       totalTicks,
                       midiMessage.delta_ticks,
                       midiMessage.status,
                       midiMessage.key,
                       midiMessage.velocity,
                       midiMessage.channel);
                i++;
            }
            totalNumNotesRead += i;
        }

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(totalNumNotesRead, 1);
    }

BOOST_AUTO_TEST_SUITE_END()