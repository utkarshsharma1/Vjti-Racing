#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>
#include <Adafruit_MLX90614.h>
#include <SPI.h>
#include <Protocentral_FDC1004.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define UPPER_BOUND  0X4000                 // max readout capacitance
#define LOWER_BOUND  (-1 * UPPER_BOUND)
#define CHANNEL 0                          // channel to be read
#define MEASURMENT 0                       // measurment channel

OneWire oneWire1(39);                                    //this is digital pin on which temperature probe is connected
DallasTemperature sensor1(&oneWire1);

OneWire oneWire2(37);                                    //this is digital pin on which temperature probe is connected
DallasTemperature sensor2(&oneWire2);

OneWire oneWire3(35);                                    //this is digital pin on which temperature probe is connected
DallasTemperature sensor3(&oneWire3);

const int chipSelect = 53; 
int capdac = 0;
File dataFile;

float rpm_1 = 0, engine_rpm = 0, sum = 0, rpm_2 = 0, diff_2 = 0, old_rpm2 = 0, shaft_rpm = 0, 
ir_temp = 0, diff_1 = 0, old_rpm1 = 0, temperature_value, sum3 = 0, avg_capaci = 0;

unsigned long timeold1 = 0, timenew_1 = 0, timeold2 = 0, timenew_2 = 0;
bool flag = 0, flag2 = 0;
float avg_rpm[5] = {0,0,0,0,0};
float avg_cap[50];
float avg_rpm2[10] = {0,0,0,0,0, 0,0,0,0,0};
String data_string = "";

void Average_rpm(float rpm);
void fuel();
void Average_rpm2(float rpm);
void Average_fuel(float capaci);
void sd_card();
void print_temperature_distance_final_rpm();

void setup(void){
  pinMode(7,INPUT_PULLUP);   //Primary 
  pinMode(10,INPUT_PULLUP);      //secondary
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  sensor1.begin();
  sensor2.begin();
  sensor3.begin();
  
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  mlx.begin();
  Wire.begin();        //i2c begin

}

void loop(void) {
  
  if(!flag){
    if(!digitalRead(10)){
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
    if(digitalRead(10)){
      flag = 0;
    }
  }

  if(!flag2){
    if(!digitalRead(7)){
      timenew_2=micros();
      diff_2=timenew_2-timeold2;
      rpm_2=(60000000.0/diff_2);
      timeold2=timenew_2;
      //if(rpm_1 < 550 && abs(old_rpm1 - rpm_1) < 150){
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
  shaft_rpm = sum / 5;
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
  engine_rpm = sum / 10;

  print_temperature_distance_final_rpm();
}

void print_temperature_distance_final_rpm(){
 /* Serial.print("  1:  ");
  Serial.print(shaft_rpm);
  
  sensor1.requestTemperatures();
  Serial.print("  Temp 1  ");
  temperature_value = sensor1.getTempCByIndex(0);
  Serial.print(temperature_value);

  sensor2.requestTemperatures();
  Serial.print("  Temp 2  ");
  temperature_value = sensor2.getTempCByIndex(0);
  Serial.print(temperature_value);

  sensor3.requestTemperatures();
  Serial.print("  Temp 3  ");
  temperature_value = sensor3.getTempCByIndex(0);
  Serial.print(temperature_value);

  Serial.print("  rpm:  ");
  Serial.print(engine_rpm);

  Serial.print("  Ambient = "); Serial.print(mlx.readAmbientTempC()); Serial.print("*C");
  ir_temp = mlx.readObjectTempC();
  Serial.print("  Object =  "); Serial.print(ir_temp); Serial.println("*C");
  */
  sd_card();
}

void sd_card(){
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  if (dataFile){
    data_string = ( " 1 " + String(engine_rpm) + " 2 " + String(shaft_rpm) + " capaci " + String(avg_capaci) + " Temp " + String(temperature_value)
       + " IR temp " + String(ir_temp) );
    dataFile.println(data_string);
    dataFile.close();
  }
  else{
    Serial.println(" Error opening data file ");
  }
}



void Average_fuel(float capaci){
  avg_cap[49] = capaci;
  sum = 0;
  for(int i =0; i < 50; i++){
    sum = sum + avg_cap[i];
  }
  for(int i =0; i<49; i++){
    avg_cap[i] = avg_cap[i+1];
  }
  avg_capaci = sum /10;
}
