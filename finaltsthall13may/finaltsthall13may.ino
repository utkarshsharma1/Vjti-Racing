#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

volatile double rpm_1=0; 
volatile unsigned long timeold1=0;
volatile unsigned long timenew_1=0;
volatile double diff_1=0;
volatile int i1 = 0;
//volatile float cvt=0;
float speed;
int address = 0;
int old_rpm;
const int chipSelect = 4;
void clearEEPROM();
void writeEeprom(int speed);

void setup()
{ 
  pinMode(3,INPUT_PULLUP);   //Primary 
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  attachInterrupt(digitalPinToInterrupt(3), rpm_fun_1,FALLING);  
  rpm_1 = 0;//Driving Pulley 
  timeold1 = 0;
  timenew_1=0;
  old_rpm = 0;
  speed = 0;
  Serial.begin(9600);
  clearEEPROM();

  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized."); */ 
}

void rpm_fun_1()
{
  timenew_1=micros();
  if(timenew_1==timeold1)
  { 
    rpm_1=0;
  }
  else
  {
    diff_1=timenew_1-timeold1;
  }
  if(i1==0)
  { 
    rpm_1=(60000000.0/diff_1);
    timeold1=timenew_1; 
  }
  i1++;
  if(i1==1)
  {
    i1=0;
  }
  diff_1 = 0;
}

void loop()
{ 
  //cvt=rpm_1/(rpm_2*11.9025);
// if(cvt>0.5&&cvt<3.2)
  Serial.println(digitalRead(3));
  if (rpm_1 < 600)
  {
    if (old_rpm != rpm_1)
    {
      Serial.print(rpm_1);
      Serial.print("       ");
      speed = (2*18*3.142*rpm_1*0.2794/(5*60));
      Serial.println(speed);
     // Serial.print("       ");
      //Serial.print(cvt);
      //Serial.print("       ");
     // Serial.println(millis());
      delay(100);
/*      writeEeprom(speed);
      File dataFile = SD.open("datalog.txt", FILE_WRITE);

      // if the file is available, write to it:
      if (dataFile) {
        dataFile.println(rpm_1);
        dataFile.close();
        // print to the serial port too:
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening datalog.txt");
      }*/
    }
    old_rpm = rpm_1;
  } 
}
    

void writeEeprom(int speed){
  int value = speed;
//  byte sen = sensor;
//  EEPROM.write(address, sen);
//  address++;
  EEPROM.write(address, value);
  address++;
  Serial.print("Data written in address ");
  Serial.println(address);
  if(address == EEPROM.length())
  {
    address = 0;
  }
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



