#include <Arduino.h>
#include "smfwriter.h"

void setup() {
    if (!SD.begin())
    {
        Serial.println("SD init failed!");
        while (true) ;
    }

    SmfWriter writer;
    writer.setFilename("test");
    writer.setTicksPerBeat(480);
    writer.writeHeader();
    writer.addSetTempo(0, 120.0);
    double microsPerTick = writer.get_microseconds_per_tick(120.0);
    writer.addNoteOnEvent(0, 1, 64, 127);
    writer.addNoteOffEvent(480, 1, 64);
    writer.addNoteOnEvent(480, 1, 64, 127);
    writer.addNoteOffEvent(480, 1, 64);
    writer.close();
}

void loop() {
    
}

#ifdef BUILD_FOR_LINUX
int main(int, char**) {
    SD.setSDCardFolderPath("./output", true);
    setup();
    //while(true) loop();
}
#endif