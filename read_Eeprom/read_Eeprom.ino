#include<EEPROM.h>
int address = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(address<EEPROM.length())
  {
    byte value = EEPROM.read(address);
    address++;
    Serial.print(address);
    Serial.print("  ");
    Serial.println(value);
  }
}
