#include <Arduino.h>
#include "smfwriter.h"

void setup() {
    SmfWriter writer;
    writer.setFilename("test.mid");
    writer.writeHeader();
    //writer.setTempo(120.0);
    //double microsPerTick = writer.get_microseconds_per_tick();

    writer.addNoteOnEvent(0, 0, 64, 127);
    writer.addNoteOnEvent(480, 0, 64, 127 );
    writer.flush();
}

void loop() {
    
}