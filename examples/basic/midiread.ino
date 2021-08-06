#include <Arduino.h>
#include "smfwriter.h"

void setup() {
    SmfWriter writer;
    writer.setFilename("test.mid");
    writer.writeHeader();
    //writer.setTempo(120.0);
    //double microsPerTick = writer.get_microseconds_per_tick();

    writer.addEvent(0, 0x90, 64, 127, 0);
    writer.addEvent(480, 0x90, 64, 127, 0 );
    writer.flush();
}

void loop() {
    
}