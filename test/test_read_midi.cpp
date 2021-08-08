#define BOOST_TEST_MODULE BasicArduinoTests
#include <boost/test/unit_test.hpp>
#include "default_test_fixture.h"
#include "smfwriter.h"
#include "midireader.h"
BOOST_AUTO_TEST_SUITE(basic_midi_write_test)

    unsigned int get_microseconds_per_tick(double beats_per_minute) {
        double micros_per_beat = 60000000.0 / beats_per_minute;
        unsigned int micros_per_tick = micros_per_beat / 480;
        return micros_per_tick;
    }

    void populateMessagesFromReader(midireader &reader, std::vector<midimessage> &messages) {
        for (int t = 0; t < reader.getNumTracks(); t++)
        {
            reader.setTrackNumber(t);
            midimessage midiMessage {};
            while (reader.read(midiMessage)) {
                messages.push_back(midiMessage);
            }
        }
    }

    BOOST_FIXTURE_TEST_CASE(can_write_basic_note, DefaultTestFixture) {

        SD.setSDCardFolderPath("output");

        system("echo ${PWD}");

        SmfWriter writer;

        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addNoteOnEvent(0, 0, 54, 127);
        writer.flush();

        midireader reader;
        reader.open(actualFileName);

        std::vector<midimessage> messages;
        populateMessagesFromReader(reader, messages);

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(messages.size(), 1);

        BOOST_CHECK_EQUAL(messages[0].channel, 0);
        BOOST_CHECK_EQUAL(messages[0].isTempoChange, false);
        BOOST_CHECK_EQUAL(messages[0].delta_ticks, 0);
        BOOST_CHECK_EQUAL(messages[0].key, 54);
        BOOST_CHECK_EQUAL(messages[0].velocity, 127);
    }

    BOOST_FIXTURE_TEST_CASE(can_write_delayed_note, DefaultTestFixture) {

        SD.setSDCardFolderPath("output");

        system("echo ${PWD}");

        SmfWriter writer;
        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addNoteOnEvent(1000, 0, 54, 127);
        writer.flush();

        midireader reader;
        reader.open(actualFileName);

        double microsPerTick = get_microseconds_per_tick(120.0);

        int totalNumNotesRead = 0;

        std::vector<midimessage> messages;
        populateMessagesFromReader(reader, messages);

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(messages.size(), 1);

        BOOST_CHECK_EQUAL(messages[0].channel, 0);
        BOOST_CHECK_EQUAL(messages[0].isTempoChange, false);
        BOOST_CHECK_EQUAL(messages[0].delta_ticks, 1000);
        BOOST_CHECK_EQUAL(messages[0].key, 54);
        BOOST_CHECK_EQUAL(messages[0].velocity, 127);

    }

    BOOST_FIXTURE_TEST_CASE(can_write_tempo_change, DefaultTestFixture) {

        SD.setSDCardFolderPath("output");

        system("echo ${PWD}");

        SmfWriter writer;
        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addSetTempo(0, 140.0);
        writer.flush();

        midireader reader;
        reader.open(actualFileName);

        double microsPerTick = get_microseconds_per_tick(120.0);

        int totalNumNotesRead = 0;

        std::vector<midimessage> messages;
        populateMessagesFromReader(reader, messages);

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(messages.size(), 1);

        BOOST_CHECK_EQUAL(messages[0].channel, 0);
        BOOST_CHECK_EQUAL(messages[0].isTempoChange, true);
        BOOST_CHECK_EQUAL(messages[0].delta_ticks, 0);
        BOOST_CHECK_EQUAL(messages[0].tempo, 140.0);
    }


BOOST_AUTO_TEST_SUITE_END()