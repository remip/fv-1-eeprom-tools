
#include <avr/pgmspace.h>
#include <Wire.h>
#include "effects.h"

#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM
ExternalEEPROM myMem;

#define PRESETLEN 512

void writepreset(uint8_t preset, const uint8_t *data, size_t len) {
  
  uint32_t offset = preset*PRESETLEN;
  
  Serial.print("Writing preset: ");
  Serial.print(preset);
  Serial.print(" memory offset: ");
  Serial.print(offset);
  Serial.print(" length: ");
  Serial.println(len);

  for( size_t i=0; i<len; i++) {
    myMem.write(offset+i, pgm_read_byte(data+i));   
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(400000); //Most EEPROMs can run 400kHz and higher

  if (myMem.begin() == false)
  {
    Serial.println("No memory detected. Freezing.");
    while (1);
  }

  Serial.println("Memory detected!");
  
  // 24LC32A
  myMem.setMemorySize(32768/8); //In bytes. 32kbits 4kbytes
  myMem.setPageSize(32); //In bytes. Has 32 byte page size.
  myMem.enablePollForWriteComplete(); //Supports I2C polling of write completion
  myMem.setPageWriteTime(5); //5 ms max write time

  writepreset(0, echo, PRESETLEN);
  writepreset(1, reverb, PRESETLEN);
  

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // blink when dump writen
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(500);
  digitalWrite(LED_BUILTIN, LOW); 
  delay(500); 
}
