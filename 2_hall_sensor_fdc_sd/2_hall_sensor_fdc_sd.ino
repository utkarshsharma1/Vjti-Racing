#include <Wire.h>
#include <OneWire.h>
//#include <DallasTemperature.h>
#include <SD.h>


#include <SPI.h>
#include <Protocentral_FDC1004.h>

#define UPPER_BOUND  0X4000                 // max readout capacitance
#define LOWER_BOUND  (-1 * UPPER_BOUND)
#define CHANNEL 0                          // channel to be read
#define MEASURMENT 0                       // measurment channel

int capdac = 0;

FDC1004 FDC;

//OneWire oneWire(2);
//DallasTemperature sensors(&oneWire);

const int chipSelect = 10; 
float avg_capaci[50] = {0};
float duration = 0, distance = 0;
File dataFile;

float rpm_1 = 0, engine_rpm = 0, rpm_2 = 0, diff_2 = 0, old_rpm2 = 0, sum = 0, shaft_rpm = 0, diff_1 = 0, old_rpm1 = 0, capaci = 0, final_capaci = 0;
unsigned long timeold1 = 0, timenew_1 = 0, timeold2 = 0, timenew_2 = 0;
bool flag = 0, flag2 = 0;
float avg_rpm[5] = {0,0,0,0,0};
float avg_rpm2[10] = {0,0,0,0,0, 0,0,0,0,0};
String data_string = "";

void Average_rpm(float rpm);
void Average_rpm2(float rpm);
void Average_capaci(float capaci);
void sd_card();
void fdc_sensor();
void print_temperature_distance_final_rpm();

void setup(void){
  pinMode(4,INPUT_PULLUP);   //Primary 
  pinMode(7,INPUT_PULLUP);      //secondary
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW); 
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH); 
   
 
  Serial.begin(9600);
  
  Wire.begin();        //i2c begin
//  sensors.begin();

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop(void) {
  
  if(!flag){
    if(!digitalRead(4)){                                    //digital pin 4 has final rpm
      timenew_1 = micros();
      diff_1=timenew_1-timeold1;
      rpm_1=(60000000.0/diff_1);
      timeold1=timenew_1;
      if(rpm_1 < 550 && abs(old_rpm1 - rpm_1) < 150){
        Average_rpm(rpm_1);
      }
      old_rpm1 = rpm_1;
      flag = 1;
    }
  }
  if(flag){
    if(digitalRead(4)){
      flag = 0;
    }
  }

  if(!flag2){
    if(!digitalRead(7)){                            //digital pin 7 has engine rpm
      timenew_2=micros();
      diff_2=timenew_2-timeold2;
      rpm_2=(60000000.0/diff_2);
      timeold2=timenew_2;
      //if(rpm_2 < 4400 && rpm_2 > 1400 && (abs(old_rpm2 - rpm_2) < 1000)){
        Average_rpm2(rpm_2);
      //}
      old_rpm2 = rpm_2;
      flag2 = 1;
    }
  }
  if(flag2){
    if(digitalRead(7)){
      flag2 = 0;
    }
  }
}

void Average_rpm(float rpm){
  avg_rpm[4] = rpm;
  sum = 0;
  for(int i =0; i < 5; i++){
    sum = sum + avg_rpm[i];
  }
  for(int i =0;i<4;i++){
    avg_rpm[i] = avg_rpm[i+1];
  }
  shaft_rpm = sum/ 5;
}

void Average_capaci(float capaci){
  avg_capaci[49] = capaci;
  sum = 0;
  for(int i = 0; i < 50; i++){
    sum = sum + avg_capaci[i];
  }
  for(int i =0; i<49; i++){
    avg_capaci[i] = avg_capaci[i+1];
  }
  final_capaci = (sum / 50);
}

void Average_rpm2(float rpm){
  
  avg_rpm2[9] = rpm;
  sum = 0;
  for(int i =0; i < 10; i++){
    sum = sum + avg_rpm2[i];
  }
  for(int i =0; i< 9; i++){
    avg_rpm2[i] = avg_rpm2[i+1];
  }
  engine_rpm = sum/10;

  fdc_sensor();

}

void fdc_sensor(){
  
  FDC.configureMeasurementSingle(MEASURMENT, CHANNEL, capdac);
  FDC.triggerSingleMeasurement(MEASURMENT, FDC1004_100HZ);

  //wait for completion
  delay(15);
  uint16_t value[2];
  if (! FDC.readMeasurement(MEASURMENT, value))
  {
    int16_t msb = (int16_t) value[0];
    int32_t capacitance = ((int32_t)457) * ((int32_t)msb); //in attofarads
    capacitance /= 1000;   //in femtofarads
    capacitance += ((int32_t)3028) * ((int32_t)capdac);

    Serial.println(((float)capacitance/1000),4);
    //Serial.print("  pf, ");
    capaci = ((float)capacitance/1000);
    Average_capaci(capaci);
    
    if (msb > UPPER_BOUND)               // adjust capdac accordingly
  {
      if (capdac < FDC1004_CAPDAC_MAX)
    capdac++;
    }
  else if (msb < LOWER_BOUND)
  {
      if (capdac > 0)
    capdac--;
    }

  }

    print_temperature_distance_final_rpm();
}


void print_temperature_distance_final_rpm(){
  Serial.print("  1:  ");
  Serial.print(shaft_rpm);
    
  Serial.print("  2:  ");
  Serial.print(engine_rpm);

  Serial.print("  cap  ");
  Serial.println(final_capaci);
  
  sd_card();
}

void sd_card(){
  dataFile = SD.open("data45.txt", FILE_WRITE);
  
  if (dataFile){
    data_string = ( " 1 " + String(engine_rpm) + " 2 " + String(shaft_rpm) + " Cap " + String(final_capaci));
    dataFile.println(data_string);
    dataFile.close();
  }
  else{
    Serial.println(" Error opening data file ");
  }
}
