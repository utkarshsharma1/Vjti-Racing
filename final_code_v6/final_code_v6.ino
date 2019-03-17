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

OneWire oneWire1(28);                                    //this is digital pin on which temperature probe is connected
DallasTemperature sensor1(&oneWire1);

OneWire oneWire2(32);                                    //this is digital pin on which temperature probe is connected
DallasTemperature sensor2(&oneWire2);

OneWire oneWire3(36);                                    //this is digital pin on which temperature probe is connected
DallasTemperature sensor3(&oneWire3);

const int chipSelect = 53; 
FDC1004 FDC;
int capdac = 0, velocity = 0, fuel_need = 0;
File dataFile;

float rpm_1 = 0, sum = 0, rpm_2 = 0, diff_2 = 0, old_rpm2 = 0, ir_temp = 0, rpm_1_final = 0,  rpm_2_final = 0,
diff_1 = 0, old_rpm1 = 0, temperature_value1 = 0, sum3 = 0, capaci = 0, temperature_value2 = 0, temperature_value3 = 0;

unsigned long timeold1 = 0, timenew_1 = 0, timeold2 = 0, timenew_2 = 0;
bool flag = 0, flag2 = 0;
float avg_rpm[10] = {0,0,0,0,0, 0,0,0 ,0,0};
float avg_cap[50];
float avg_rpm2[5] = {0,0,0,0,0};
String data_string = "";

int Average_rpm(float rpm);
void fuel();
void Average_rpm2(float rpm);
float Average_fuel(float capaci);
void sd_card();
void print_temperature_distance_final_rpm();

void setup(void){
  pinMode(7,INPUT_PULLUP);   //Primary, change this as required 
  pinMode(3,INPUT_PULLUP);      //secondary,  change this as required
  Serial.begin(9600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/

  sensor1.begin();
  sensor2.begin();
  sensor3.begin();
  
//  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
//    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  //Serial.println("card initialized.");
  mlx.begin();
  Wire.begin();        //i2c begin
  
  dataFile = SD.open("log1.txt", FILE_WRITE);
  
  if (dataFile){
    data_string = ("New Start");
    dataFile.println(data_string);
    dataFile.close();
  }
  //else{
    //Serial.println(" Error opening data file ");
  //}
}


void loop(void) {

  
  if(!flag){
    if(!digitalRead(7)){                      //7 is primary
      timenew_1 = micros();
      diff_1=timenew_1-timeold1;
      rpm_1=(60000000.0/diff_1);
      timeold1=timenew_1;
      //rpm_1_final = 10;
      //if(rpm_1 < 4400 && rpm_1 > 1400 && (abs(old_rpm1 - rpm_1) < 1000)){
        rpm_1_final = rpm_1;
      //}
      old_rpm1 = rpm_1;
      flag = 1;
    }
  }
  if(flag){
    if(digitalRead(7)){
      flag = 0;
    }
  }

  if(!flag2){
    if(!digitalRead(3)){                                          //3 is secondary
      timenew_2=micros();
      diff_2=timenew_2-timeold2;
      rpm_2=(60000000.0/diff_2);
      timeold2=timenew_2;
      if(rpm_2 < 550 && abs(old_rpm2 - rpm_2) < 100){
        Average_rpm2(rpm_2);
      }
      old_rpm2 = rpm_2;
      flag2 = 1;
    }
  }
  if(flag2){
    if(digitalRead(3)){
      flag2 = 0;
    }
  }
  int a = velocity;
 // Serial.println(a);
  Serial.print("n0.val=");
  Serial.print(a);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  if(fuel_need==1){  // If the variable is greater than 119...
    Serial.print("vis j0,1");  // This is to hide or show an object. The name of the object comes before the comma, and after the comma goes a 0 to hide or 1 to show.
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
  }
  else{  // Since condition was false, do the following:
    Serial.print("vis j0,0");  // This is to hide or show an object. The name of the object comes before the comma, and after the comma goes a 0 to hide or 1 to show.    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
  }
}

int Average_rpm(float rpm_1){
  Serial.println("ggdgdb");
  avg_rpm[9] = rpm_1;
  sum = 0;
  for(int i =0; i < 10; i++){
    sum = sum + avg_rpm[i];
  }
  for(int i =0; i<9; i++){
    avg_rpm[i] = avg_rpm[i+1];
  }
  return (int)(sum / 10);
}

void Average_rpm2(float rpm_2){
  avg_rpm2[4] = rpm_2;
  sum = 0;
  for(int i =0; i < 5; i++){
    sum = sum + avg_rpm2[i];
  }
  for(int i =0; i< 4; i++){
     avg_rpm2[i] = avg_rpm2[i+1];
  }
  rpm_2_final = (int)(sum / 5);
  //velocity = (int)(2*18*3.142*rpm_2*0.2592/(5*60*10.27));
  //velocity = (int)(rpm_2*0.009507370);
  velocity = (int)(rpm_2_final * 0.097640689);
  fuel();
  print_temperature_distance_final_rpm();
}

void print_temperature_distance_final_rpm(){
//  Serial.print("  1:  ");
//  Serial.print(rpm_1_final);

//  Serial.print("  VELO  ");
//  Serial.print(velocity, DEC);
//  
  sensor1.requestTemperatures();
//  Serial.print("  Temp 1  ");
  temperature_value1 = sensor1.getTempCByIndex(0);
   //Serial.print(temperature_value1);
//
  sensor2.requestTemperatures();
//  Serial.print("  Temp 2  ");
  temperature_value2 = sensor2.getTempCByIndex(0);
    //Serial.print(temperature_value2);//
  sensor3.requestTemperatures();
//  Serial.print("  Temp 3  ");
  temperature_value3 = sensor3.getTempCByIndex(0);
    //Serial.print(temperature_value3);
//
//  Serial.print("  rpm:  ");
 // Serial.println(rpm_2);
//
//  Serial.print("  cap  ");
//  Serial.print(capaci);  
//
//  //Serial.print("  Ambient = "); Serial.print(mlx.readAmbientTempC()); Serial.print("*C");
    
  ir_temp = mlx.readObjectTempC();
//  Serial.print("  Object =  "); Serial.print(ir_temp); Serial.println("*C");
  
  sd_card();
}

void sd_card(){
  dataFile = SD.open("log1.txt", FILE_WRITE);
  
  if (dataFile){
    data_string = ( " 1 " + String(rpm_1_final) + " 2 " + String(rpm_2_final) + " speed " + String(velocity) + " capaci " + String(capaci) + " Temp1 " + 
    String(temperature_value1) + " Temp2 " + String(temperature_value2) + " Temp3 " + String(temperature_value3) 
       + " IR temp " + String(ir_temp) );
    dataFile.println(data_string);
    dataFile.close();
  }
  //else{
    //Serial.println(" Error opening data file ");
  //}
}

void fuel(){
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

    capaci = ((float)capacitance/1000);
    capaci = Average_fuel(capaci);
    if(capaci < 196)
    {
      fuel_need = 1;
    }
    else
    {
      fuel_need = 0;
    }

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
}

float Average_fuel(float capaci){
  avg_cap[49] = capaci;
  sum = 0;
  for(int i =0; i < 50; i++){
    sum = sum + avg_cap[i];
  }
  for(int i =0; i<49; i++){
    avg_cap[i] = avg_cap[i+1];
  }
  return (sum /10);
}
