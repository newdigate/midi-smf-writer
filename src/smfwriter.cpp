#include "smfwriter.h"
#include <vector>

const byte header[] = {
     0x4d, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06,
     0x00, 0x00, // single-track format
     0x00, 0x01, // one track
     0x00, 0x10, // 16 ticks per quarter
     0x4d, 0x54, 0x72, 0x6B
};

SmfWriter::SmfWriter(){
}

char* SmfWriter::getFilename() {
  return &_filename[0];
}

void SmfWriter::write_buf_int(unsigned int data) {
  //Serial.printf("%x\n", data);
  byte b = data >> 24;
  //Serial.printf("%x.", b);
  write_buf_byte( b );
  
  b = data >> 16;
  //Serial.printf("%x.", b);
  write_buf_byte( b );
   
  b = data >> 8;
  //Serial.printf("%x.", b);
  write_buf_byte( b );

  b = data;
  //Serial.printf("%x\n", b);
  write_buf_byte( b );
}

void SmfWriter::write_buf_byte(byte b) {
  if (_bufferPos > 40) {
    flush();
  }
  _buffer[_bufferPos++] = b;
}
    
void SmfWriter::setFilename(const char* filename) {
  flush();

  sprintf(_filename, "%s.mid", filename);
  _filename[strlen(filename)] = 0;
  int count = 1;
  while (SD.exists(_filename)) {
    //Serial.printf("'%s' already exists...\n", _filename);   
    sprintf(_filename, "%s%i.mid", filename, count);
    count++;
  }
  //Serial.printf("using filename '%s'\n", _filename);
}

void SmfWriter::writeHeader() {
  for (unsigned int i=0; i<sizeof(header); i++)
    write_buf_byte(header[i]);
  flush();
}

void SmfWriter::addEvent(unsigned int deltaticks, byte type, byte data1, byte data2, byte channel) {

  
  //Serial.printf("delta ticks= %d :type= %x, data1=%x, data2=%x, channel=%x\n", deltaticks, type, data1, data2, channel);
  if (deltaticks < 128) {
    write_buf_byte(deltaticks);
    trackSize += 1;
    //Serial.printf("[", deltaticks);
  } else {
    //Serial.print("[");

    std::vector<byte*> q;
    byte b[4];
    
    for (int i = 3; i >= 0; i--) {
        b[i] = (byte)(deltaticks & 0x7f);
  
        if(i < 3)
            b[i] |= 0x80;

        q.push_back(&b[i]);
        //Serial.printf("push %d: /%x/ \n", i, b[i]);
        //trackSize += 1;
        
        deltaticks >>= 7;
  
        if (deltaticks < 1)
            break;
    }

    while (q.size() > 0) {
      byte b = *(q.back());
      q.pop_back();
      //Serial.printf("pop /%x/ \n", b);
      
      write_buf_byte(b);
      trackSize += 1;

      //char buffer [9];
      //itoa (b,buffer,2);
      //Serial.printf ("%x binary: %s\n", b, buffer);
    }
  }
  byte z = type | channel;
  //Serial.printf("] - %x %x %x \n", z, data1, data2);
  //write_buf_int(ticks);
  write_buf_byte(z);
  write_buf_byte(data1);
  write_buf_byte(data2);
  trackSize += 3;
}

void SmfWriter::flush() {
    if (_bufferPos == 0) return;
    File data = SD.open(_filename, O_WRITE);
    if (!data) {
        char *notAbleToOpen = const_cast<char *>("Not able to open ");
        Serial.print(notAbleToOpen);
        Serial.print(_filename);
        Serial.println();
        return;
    }

    for (byte b = 0; b < _bufferPos; b++) {
        data.write(_buffer[b]);
    }
    _bufferPos = 0;

    data.seek(18);
    write_buf_int(trackSize);
    for (byte b = 0; b < _bufferPos; b++) {
        data.write(_buffer[b]);
    }
    _bufferPos = 0;

    data.close();
}