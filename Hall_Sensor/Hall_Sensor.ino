#include <EEPROM.h>

volatile float half_revolutions1;
float rpm1;
float timeold1;
volatile float half_revolutions2;
float rpm2;
float timeold2;
int address = 0;
float speed = 0;
float cvt = 0;

void clearEEPROM();
void writeEeprom(float speed);

void setup()
{
  Serial.begin(115200);
  pinMode(3,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), magnet_detect1, RISING);   //Initialize the intterrupt pin (Arduino digital pin 2)
  attachInterrupt(digitalPinToInterrupt(2), magnet_detect2, RISING);
  half_revolutions1 = 0;
  rpm1 = 0;
  timeold1 = 0;
  half_revolutions2 = 0;
  rpm2 = 0;
  timeold2 = 0;
  clearEEPROM();
/*  half_revolutions2 = 0;
  rpm2 = 0;
  timeold2 = 0;*/
}


void loop()//Measure RPM
{
  if (half_revolutions1 >= 20) { 
    rpm1 = 30*1000/(millis() - timeold1)*half_revolutions1;
    timeold1 = millis();
    half_revolutions1 = 0;
    Serial.print("1   ");
    Serial.print(rpm1,DEC);
    speed = (2*18*3.142*rpm1*0.2794/(5*60));
    Serial.print("   ");
    Serial.println(speed ,DEC);
    writeEeprom(speed, 1); 
  }
  
  if (half_revolutions2 >= 20) { 
    rpm2 = 30*1000/(millis() - timeold2)*half_revolutions2;
    timeold2 = millis();
    half_revolutions2 = 0;
    Serial.print("2   ");
    Serial.print(rpm2,DEC);
    speed = (2*18*3.142*rpm2*0.2794/(5*60));
    Serial.print("   ");
    Serial.println(speed ,DEC);
    writeEeprom(speed, 2); 
  }
}

void magnet_detect1()//This function is called whenever a magnet/interrupt is detected by the arduino
{
  half_revolutions1++;
}

void writeEeprom(int speed, byte sensor){
  byte value = speed;
  byte sen = sensor;
  EEPROM.write(address, sen);
  address++;
  EEPROM.write(address, value);
  address++;
  Serial.print("Data written in address ");
  Serial.println(address);
  /*if(address == EEPROM.length())
  {
    address = 0;
  }*/
}

void clearEEPROM()
{
  for(int i = 0; i<EEPROM.length(); i++){
    //if(EEPROM.read(i)!=0){
      EEPROM.write(i,0);
  //  }
  }
  Serial.println("EEPROM erased");
  address = 0;
}

void magnet_detect2()//This function is called whenever a magnet/interrupt is detected by the arduino
{
  half_revolutions2++;
}
