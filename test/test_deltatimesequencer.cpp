#include <boost/test/unit_test.hpp>
#include "default_test_fixture.h"
#include "DeltaTimeSequencer.h"
#define ONE_SECOND 1000000
BOOST_AUTO_TEST_SUITE(basic_DeltaTimeSequencertest)

    BOOST_FIXTURE_TEST_CASE(can_sequence, DefaultTestFixture) {

        SD.setSDCardFolderPath("./output", true);

        unsigned long microseconds = 10 * ONE_SECOND;
        unsigned long microsPerTick = DeltaTimeSequencer::calculateMicrosPerTick(60.0, 1);
        DeltaTimeSequencer deltaTimeSequencer(microsPerTick, false);
        deltaTimeSequencer.start(microseconds);
        unsigned long timeMicroseconds = deltaTimeSequencer.getMicroseconds(microseconds);
        unsigned long timeInactivity = deltaTimeSequencer.getInactivityMicros(microseconds);
        BOOST_CHECK_EQUAL(timeMicroseconds, 0);
        BOOST_CHECK_EQUAL(timeInactivity, 0);
        Serial.printf("Start: microseconds=%d\ttime=%d\tinactivity=%d\n", microseconds, timeMicroseconds, timeInactivity);

        microseconds += ONE_SECOND;
        auto delta = deltaTimeSequencer.get_delta(microseconds);
        timeMicroseconds = deltaTimeSequencer.getMicroseconds(microseconds);
        timeInactivity = deltaTimeSequencer.getInactivityMicros(microseconds);
        Serial.printf("\t\tdelta=%d\tmicroseconds=%d\ttime=%d\tinactivity=%d\n\n", delta, microseconds, timeMicroseconds, timeInactivity);
        BOOST_CHECK_EQUAL(timeMicroseconds, ONE_SECOND );
        BOOST_CHECK_EQUAL(timeInactivity, 0);
        BOOST_CHECK_EQUAL(delta, 1);

        microseconds += ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        timeMicroseconds = deltaTimeSequencer.getMicroseconds(microseconds);
        timeInactivity = deltaTimeSequencer.getInactivityMicros(microseconds);
        Serial.printf("\t\tdelta=%d\tmicroseconds=%d\ttime=%d\tinactivity=%d\n\n", delta, microseconds, timeMicroseconds, timeInactivity);
        BOOST_CHECK_EQUAL(timeMicroseconds, 2 * ONE_SECOND );
        BOOST_CHECK_EQUAL(timeInactivity, 0);
        BOOST_CHECK_EQUAL(delta, 1);

        microseconds += 2 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        timeMicroseconds = deltaTimeSequencer.getMicroseconds(microseconds);
        timeInactivity = deltaTimeSequencer.getInactivityMicros(microseconds);
        Serial.printf("\t\tdelta=%d\tmicroseconds=%d\ttime=%d\tinactivity=%d\n\n", delta, microseconds, timeMicroseconds, timeInactivity);
        BOOST_CHECK_EQUAL(timeMicroseconds, 4 * ONE_SECOND );
        BOOST_CHECK_EQUAL(timeInactivity, 0);
        BOOST_CHECK_EQUAL(delta, 2);

        microseconds += 10 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        timeMicroseconds = deltaTimeSequencer.getMicroseconds(microseconds);
        timeInactivity = deltaTimeSequencer.getInactivityMicros(microseconds);
        Serial.printf("\t\tdelta=%d\tmicroseconds=%d\ttime=%d\tinactivity=%d\n\n", delta, microseconds, timeMicroseconds, timeInactivity);
        BOOST_CHECK_EQUAL(timeMicroseconds, 14 * ONE_SECOND );
        BOOST_CHECK_EQUAL(timeInactivity, 0);
        BOOST_CHECK_EQUAL(delta, 10);

        microseconds += 3 * ONE_SECOND;
        deltaTimeSequencer.pause(microseconds);
        Serial.printf("Paused micros=%d\n", microseconds);

        microseconds += 8 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds); // should be 3 seconds;
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 3);

        microseconds += 7 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds); // should be 0;
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 0);

        microseconds += 17 * ONE_SECOND;
        deltaTimeSequencer.unpause(microseconds);
        Serial.printf("Unpaused micros=%d\n", microseconds);

        microseconds += 5 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds); // should be 5;
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 5);

        microseconds += ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 1);

        microseconds += 0;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 0);

        microseconds += 1 * ONE_SECOND;
        deltaTimeSequencer.pause(microseconds);
        Serial.printf("Paused micros=%d\n", microseconds);

        microseconds += 17 * ONE_SECOND;
        deltaTimeSequencer.unpause(microseconds);
        Serial.printf("Unpaused micros=%d\n", microseconds);

        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 1);

        microseconds += 0;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 0);
    }

    BOOST_FIXTURE_TEST_CASE(can_sequence_pause, DefaultTestFixture) {

        SD.setSDCardFolderPath("./output", true);

        unsigned microseconds = 10 * ONE_SECOND;
        unsigned microsPerTick = DeltaTimeSequencer::calculateMicrosPerTick(60.0, 1);
        DeltaTimeSequencer deltaTimeSequencer(microsPerTick, false);
        deltaTimeSequencer.start(microseconds);
        Serial.printf("Start time: %d\n", microseconds);

        microseconds += ONE_SECOND;
        auto delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 1);

        microseconds += ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 1);

        microseconds += 2 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 2);

        microseconds += 10 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 10);

        microseconds += 3 * ONE_SECOND;
        deltaTimeSequencer.pause(microseconds);
        Serial.printf("Paused micros=%d\n", microseconds);

        microseconds += 17 * ONE_SECOND;
        deltaTimeSequencer.unpause(microseconds);
        Serial.printf("Unpaused micros=%d\n", microseconds);

        microseconds += 5 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds); // should be 5 + 3;
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 8);

    }

    BOOST_FIXTURE_TEST_CASE(can_sequence_pause1, DefaultTestFixture) {

        SD.setSDCardFolderPath("./output", true);

        unsigned microseconds = 10 * ONE_SECOND;
        unsigned microsPerTick = DeltaTimeSequencer::calculateMicrosPerTick(60.0, 1);
        DeltaTimeSequencer deltaTimeSequencer(microsPerTick, false);
        deltaTimeSequencer.start(microseconds);
        Serial.printf("Start time: %d\n", microseconds);

        microseconds += ONE_SECOND;
        auto delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 1);

        microseconds += ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 1);

        microseconds += 2 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 2);

        microseconds += 10 * ONE_SECOND;
        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 10);

        deltaTimeSequencer.pause(microseconds);
        Serial.printf("Paused micros=%d\n", microseconds);

        delta = deltaTimeSequencer.get_delta(microseconds);
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 0);

        microseconds += 17 * ONE_SECOND;
        deltaTimeSequencer.unpause(microseconds);
        Serial.printf("Unpaused micros=%d\n", microseconds);

        delta = deltaTimeSequencer.get_delta(microseconds); // should be 0;
        Serial.printf("  micros=%d delta=%d\n", microseconds, delta);
        BOOST_CHECK_EQUAL(delta, 0);

    }


BOOST_AUTO_TEST_SUITE_END()