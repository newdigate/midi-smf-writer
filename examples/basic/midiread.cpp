#include <Arduino.h>
#include "smfwriter.h"

void setup() {
    SmfWriter writer;
    writer.setFilename("test");
    writer.writeHeader();
    writer.addSetTempo(0, 120.0);
    double microsPerTick = writer.get_microseconds_per_tick(120.0);

    writer.addNoteOnEvent(0, 0, 64, 127);
    writer.addNoteOnEvent(480, 0, 64, 127 );
    writer.flush();
}

void loop() {
    
}

//#ifdef BUILD_FOR_LINUX
int main(int, char**) {
    SD.setSDCardFolderPath("/Users/moolet/Development/github/newdigate/midi-smf-writer/cmake-build-debug");
    setup();
    //while(true) loop();
}
//#endif