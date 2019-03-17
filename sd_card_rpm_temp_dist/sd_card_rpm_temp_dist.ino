#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

const int chipSelect = 53, trigPin1 = 3, echoPin1 = 4; 
float avg_dist[10] = {0,0,0,0,0,0,0,0,0,0};
float duration = 0, distance = 0;
File dataFile;

float rpm_1 = 0, engine_rpm = 0, sum1 = 0, rpm_2 = 0, diff_2 = 0, old_rpm2 = 0, sum2 = 0;
float shaft_rpm = 0, diff_1 = 0, old_rpm1 = 0, temperature_value, sum3 = 0, dist = 0;
unsigned long timeold1 = 0, timenew_1 = 0, timeold2 = 0, timenew_2 = 0;
bool flag = 0, flag2 = 0;
float avg_rpm[5] = {0,0,0,0,0};
float avg_rpm2[10] = {0,0,0,0,0, 0,0,0,0,0};
String data_string = "";

void Average_rpm(float rpm);
void Average_rpm2(float rpm);
float Average_dist(float dist);
void sd_card();
void print_temperature_distance_final_rpm();

void setup(void){
  pinMode(7,INPUT_PULLUP);   //Primary 
  pinMode(10,INPUT_PULLUP);      //secondary
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);

  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  sensors.begin();
  pinMode(48,OUTPUT);
  pinMode(49,OUTPUT);
  digitalWrite(48, LOW);
  digitalWrite(49, HIGH);  
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("READY");
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  
  data_string = ( " 1 " + String(engine_rpm) + " 2 " + String(shaft_rpm) + " Dist " + String(dist) + " Temp " + String(temperature_value));
  dataFile.println(data_string);
  dataFile.close();
  Serial.println(data_string);
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
      if(rpm_2 < 4400 && rpm_2 > 1400 && (abs(old_rpm2 - rpm_2) < 1000)){
        Average_rpm2(rpm_2);
      }
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
  sum1 = 0;
  for(int i =0; i < 5; i++){
    sum1 = sum1 + avg_rpm[i];
  }
  for(int i =0;i<4;i++){
    avg_rpm[i] = avg_rpm[i+1];
  }
  shaft_rpm = sum1 / 5;
}

void Average_rpm2(float rpm){
  avg_rpm2[9] = rpm;
  sum2 = 0;
  for(int i =0; i < 10; i++){
    sum2 = sum2 + avg_rpm2[i];
  }
  for(int i =0; i< 9; i++){
    
    avg_rpm2[i] = avg_rpm2[i+1];
  }
  engine_rpm = sum2/10;

  print_temperature_distance_final_rpm();
}

void print_temperature_distance_final_rpm(){
  Serial.print("  1:  ");
  Serial.print(shaft_rpm);
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration = pulseIn(echoPin1, HIGH);
  distance = (duration*.034)/2;
  Serial.print("  Dist1: ");
  dist = Average_dist ( distance); 
  Serial.print( dist);
  
  sensors.requestTemperatures();
  Serial.print("  Temp  ");
  temperature_value = sensors.getTempCByIndex(0);
  Serial.print(temperature_value);

  Serial.print("  2:  ");
  Serial.println(engine_rpm);

 // sd_card();
}

void sd_card(){
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile){
    data_string = ( " 1 " + String(engine_rpm) + " 2 " + String(shaft_rpm) + " Dist " + String(dist) + " Temp " + String(temperature_value));
    dataFile.println(data_string);
    dataFile.close();
    Serial.println(data_string);
  }
  else{
    Serial.println(" Error opening data file ");
  }
 // delay(2000);
}

float Average_dist(float dist){
  avg_dist[9] = dist;
  sum3 = 0;
  for(int i =0; i < 10; i++){
    sum3 = sum3 + avg_dist[i];
  }
  for(int i =0;i<9;i++){
    avg_dist[i] = avg_dist[i+1];
  }
  return dist = sum3 /10;
}
