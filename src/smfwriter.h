#ifndef NEWDIGATE_MIDI_SMF_WRITER_SMFWRITER_H
#define NEWDIGATE_MIDI_SMF_WRITER_SMFWRITER_H

#include <Arduino.h>
#include <SD.h>

class SmfWriter
{
  public:
    SmfWriter();
    void setFilename(const char* filename);
    char* getFilename();
    void writeHeader();
    void addEvent(unsigned int deltaticks, byte a, byte b, byte c, byte d);
    void flush();
    
  private:
    byte _buffer[50];
    byte _bufferPos = 0;
    char _filename[80];
    unsigned long trackSize = 0;

    void write_buf_int(unsigned int data);
    void write_buf_byte(byte a);
};

#endif // NEWDIGATE_MIDI_SMF_WRITER_SMFWRITER_H