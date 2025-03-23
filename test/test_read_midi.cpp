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

    void populateMessagesFromReader(midireader &reader, std::vector<smfmidimessage*> &messages) {
        for (int t = 0; t < reader.getNumTracks(); t++)
        {
            reader.setTrackNumber(t);
            smfmidimessage *message; 
            while ((message = reader.read()) != nullptr) {
                messages.push_back(message);
            }
        }
    }

    BOOST_FIXTURE_TEST_CASE(can_write_basic_note, DefaultTestFixture) {

        SD.setSDCardFolderPath("./output", true);

        SmfWriter writer;

        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addNoteOnEvent(0, 0, 54, 127);
        writer.close();

        midireader reader;
        reader.open(actualFileName);

        std::vector<smfmidimessage*> messages;
        populateMessagesFromReader(reader, messages);

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(messages.size(), 1);

        BOOST_CHECK_EQUAL(messages[0]->delta_ticks, 0);
        BOOST_CHECK_EQUAL(messages[0]->getMessageType(), smftype_channelvoicemessage);
        BOOST_CHECK_EQUAL( ((smfchannelvoicemessage*) messages[0])->status, 0x90);
        BOOST_CHECK_EQUAL( ((smfchannelvoicemessage*) messages[0])->data1, 54);
        BOOST_CHECK_EQUAL( ((smfchannelvoicemessage*) messages[0])->data2, 127);
    }

    BOOST_FIXTURE_TEST_CASE(can_write_delayed_note, DefaultTestFixture) {

        SD.setSDCardFolderPath("./output", true);

        SmfWriter writer;
        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addNoteOnEvent(1000, 0, 54, 127);
        writer.close();

        midireader reader;
        reader.open(actualFileName);

        int totalNumNotesRead = 0;

        std::vector<smfmidimessage*> messages;
        populateMessagesFromReader(reader, messages);

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(messages.size(), 1);

        BOOST_CHECK_EQUAL(messages[0]->delta_ticks, 1000);
        BOOST_CHECK_EQUAL(messages[0]->getMessageType(), smftype_channelvoicemessage);
        BOOST_CHECK_EQUAL( ((smfchannelvoicemessage*) messages[0])->status, 0x90);
        BOOST_CHECK_EQUAL( ((smfchannelvoicemessage*) messages[0])->data1, 54);
        BOOST_CHECK_EQUAL( ((smfchannelvoicemessage*) messages[0])->data2, 127);
    }

    BOOST_FIXTURE_TEST_CASE(can_write_tempo_change, DefaultTestFixture) {

        SD.setSDCardFolderPath("./output", true);

        SmfWriter writer;
        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addSetTempo(0, 140.0);
        writer.close();

        midireader reader;
        reader.open(actualFileName);

        int totalNumNotesRead = 0;

        std::vector<smfmidimessage*> messages;
        populateMessagesFromReader(reader, messages);

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(messages.size(), 1);
        BOOST_CHECK_EQUAL(messages[0]->delta_ticks, 0);
        BOOST_CHECK_EQUAL(messages[0]->getMessageType(), smftype_settempomessage);
        BOOST_CHECK_EQUAL( ((smfsettempomessage*) messages[0])->microseconds_per_quarter_note, 428571);
        BOOST_CHECK_EQUAL( (long) ((smfsettempomessage*) messages[0])->getTempo(), 140);
    }

    BOOST_FIXTURE_TEST_CASE(can_write_program_change, DefaultTestFixture) {

        SD.setSDCardFolderPath("./output", true);

        SmfWriter writer;
        char *actualFileName = new char[50];

        writer.setFilename("test");
        sprintf(actualFileName, "%s", writer.getFilename());
        writer.writeHeader();
        writer.addProgramChange(0, 100, 0);
        writer.close();
        midireader reader;
        reader.open(actualFileName);

        int totalNumNotesRead = 0;

        std::vector<smfmidimessage *> messages;
        populateMessagesFromReader(reader, messages);

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(messages.size(), 1);

        BOOST_CHECK_EQUAL(messages[0]->delta_ticks, 0);
        BOOST_CHECK_EQUAL(messages[0]->getMessageType(), smftype_channelvoicemessage);
        BOOST_CHECK_EQUAL( ((smfchannelvoicemessage*) messages[0])->status, 0xD0);
        BOOST_CHECK_EQUAL( ((smfchannelvoicemessage*) messages[0])->data1,  100);
    }


BOOST_AUTO_TEST_SUITE_END()