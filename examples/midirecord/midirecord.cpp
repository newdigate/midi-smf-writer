#include <Arduino.h>
#include <MIDI.h>
#include "smfwriter.h"
#include "DeltaTimeSequencer.h"

#ifdef BUILD_FOR_LINUX
#include "RtMidiMIDI.h"
#include "RtMidiTransport.h"


MIDI_CREATE_RTMIDI_INSTANCE(RtMidiMIDI, rtMIDI,  MIDI);
#else
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

SmfWriter writer;
double atempo = 120.0;
unsigned short resolution = 480;
unsigned long lastMicroseconds = 0;

DeltaTimeSequencer deltaTimeSequencer(atempo, resolution, true);

void handleNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
    const auto deltaTicks = deltaTimeSequencer.get_delta(micros());
    writer.addNoteOnEvent(deltaTicks, channel, pitch, velocity);
}
void handleNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
    const auto deltaTicks = deltaTimeSequencer.get_delta(micros());
    writer.addNoteOffEvent(deltaTicks, channel, pitch);
}
void handleAfterTouchPoly(byte inChannel, byte inNote, byte inValue) {
    const auto deltaTicks = deltaTimeSequencer.get_delta(micros());
    writer.addAfterTouch(deltaTicks, inNote, inValue, inChannel);
}
void handleControlChange(byte inChannel, byte inNumber, byte inValue) {
    const auto deltaTicks = deltaTimeSequencer.get_delta(micros());
    writer.addControlChange(deltaTicks, inNumber, inValue, inChannel);
}
void handleProgramChange(byte inChannel, byte inNumber) {
    const auto deltaTicks = deltaTimeSequencer.get_delta(micros());
    writer.addProgramChange(deltaTicks, inNumber, inChannel);
}
void handleAfterTouchChannel(byte inChannel, byte inPressure) {
    const auto deltaTicks = deltaTimeSequencer.get_delta(micros());
    writer.addAfterTouch(deltaTicks, inPressure, inChannel);
}
void handlePitchBend(byte inChannel, int inValue) {
    const auto deltaTicks = deltaTimeSequencer.get_delta(micros());
    writer.addPitchBend(deltaTicks, inValue, inChannel);
}
void reset() {
    writer.close();
    deltaTimeSequencer.stop();
    lastMicroseconds = 0;
    writer.setFilename("test");
    Serial.println(writer.getFilename());
    writer.setTicksPerBeat(resolution);
    writer.writeHeader();
    writer.addSetTempo(0, atempo);
}

void setup() {
    Serial.begin(9600);

    if (!SD.begin())
    {
        Serial.println("SD init failed!");
        while (true) ;
    }

    MIDI.setHandleNoteOn( handleNoteOn );
    MIDI.setHandleNoteOff( handleNoteOff );
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleAfterTouchChannel(handleAfterTouchChannel);
    MIDI.setHandleAfterTouchPoly(handleAfterTouchPoly);

    reset();

    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
    lastMicroseconds = 0;
}

void loop() {
    unsigned long currentMicros = micros();
    while(MIDI.read()){ lastMicroseconds = currentMicros; };
    if (lastMicroseconds > 0 && currentMicros > lastMicroseconds + 10000000) {
        Serial.println("Inactivity....");
        reset();
    }
}

#ifdef BUILD_FOR_LINUX
int main(int, char**) {
    SD.setSDCardFolderPath("./output", true);
    setup();
    while(true) loop();
}
#endif