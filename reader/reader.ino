
#include <Wire.h>

#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM
ExternalEEPROM myMem;

#define PATCHLEN 512

void readpreset(uint8_t preset) {

  uint32_t offset = preset*PATCHLEN;
  char buf[10];

  Serial.print("Reading preset: ");
  Serial.print(preset);
  Serial.print(" memory offset: ");
  Serial.println(offset);
  Serial.println("");
  
  Serial.print("const uint8_t preset");
  Serial.print(preset);
  Serial.println("[] = {");
  for ( uint32_t x=offset; x < offset+PATCHLEN; x = x+16) {
    for( uint32_t y=0; y<16; y++) {
        byte val = myMem.read(x+y);
        sprintf(buf, "0x%02X, ", val);        
        Serial.print(buf);
    }
    Serial.println("");
  }
  Serial.println("};");
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
  myMem.setMemorySize(32768/8); //In bytes. 512kbit = 64kbyte
  myMem.setPageSize(32); //In bytes. Has 128 byte page size.
  myMem.enablePollForWriteComplete(); //Supports I2C polling of write completion
  myMem.setPageWriteTime(5); //3 ms max write time

  readpreset(0);
  readpreset(1);
  readpreset(2);
  

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // blink when dump done
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(500);
  digitalWrite(LED_BUILTIN, LOW); 
  delay(500); 
}
