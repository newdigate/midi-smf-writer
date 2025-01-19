#include <Arduino.h>
#include <MIDI.h>
#include "smfwriter.h"

#ifdef BUILD_FOR_LINUX
#include "RtMidiMIDI.h"
#include "RtMidiTransport.h"
MIDI_CREATE_RTMIDI_INSTANCE(RtMidiMIDI, rtMIDI,  MIDI);
using TMidiTransport = RtMidiTransport<RtMidiMIDI>;
using TMidi = midi::MidiInterface<TMidiTransport>;
#else
using TMidiTransport = midi::SerialMIDI<arduino::HardwareSerial>;
using TMidi = midi::MidiInterface<TMidiTransport>;
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

SmfWriter writer;
unsigned long startMicroseconds = 0, lastTick = 0, lastMicroseconds = 0;
double tempo = 120.0;

unsigned update_ticks_and_return_delta() {
    const auto currentMicros = micros();
    if (startMicroseconds == 0) {
        Serial.println("!***** Recording Started!");
        startMicroseconds = currentMicros;
        lastMicroseconds = currentMicros;
        lastTick = 0;
        return 0;
    }

    const auto deltaMicros = currentMicros - startMicroseconds;
    unsigned ticks = deltaMicros / writer.getTicksPerBeat();
    auto delta = ticks - lastTick;
    lastTick = ticks;
    lastMicroseconds = currentMicros;
    return delta;
}


void handleNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
    const auto deltaTicks = update_ticks_and_return_delta();
    writer.addNoteOnEvent(deltaTicks, channel, pitch, velocity);
}
void handleNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
    const auto deltaTicks = update_ticks_and_return_delta();
    writer.addNoteOffEvent(deltaTicks, channel, pitch);
}
void handleAfterTouchPoly(byte inChannel, byte inNote, byte inValue) {
    const auto deltaTicks = update_ticks_and_return_delta();
    writer.addAfterTouch(deltaTicks, inNote, inValue, inChannel);
}
void handleControlChange(byte inChannel, byte inNumber, byte inValue) {
    const auto deltaTicks = update_ticks_and_return_delta();
    writer.addControlChange(deltaTicks, inNumber, inValue, inChannel);
}
void handleProgramChange(byte inChannel, byte inNumber) {
    const auto deltaTicks = update_ticks_and_return_delta();
    writer.addProgramChange(deltaTicks, inNumber, inChannel);
}
void handleAfterTouchChannel(byte inChannel, byte inPressure) {
    const auto deltaTicks = update_ticks_and_return_delta();
    writer.addAfterTouch(deltaTicks, inPressure, inChannel);
}
void handlePitchBend(byte inChannel, int inValue) {
    const auto deltaTicks = update_ticks_and_return_delta();
    writer.addPitchBend(deltaTicks, inValue, inChannel);
}
void reset() {
    writer.close();
    startMicroseconds = 0;
    lastTick = 0;
    lastMicroseconds = 0;
    writer.setFilename("test");
    Serial.println(writer.getFilename());
    writer.setTicksPerBeat(480);
    writer.writeHeader();
    writer.addSetTempo(0, tempo);
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
}

void loop() {
    while(MIDI.read()){};
    unsigned long currentMicros = micros();
    if (lastMicroseconds > 0 && currentMicros > lastMicroseconds + 10000000) {
        Serial.println("Inactivity....");
        reset();
    }
}

#ifdef BUILD_FOR_LINUX
int main(int, char**) {
    SD.setSDCardFolderPath("./output");
    setup();
    while(true) loop();
}
#endif