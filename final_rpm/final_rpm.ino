#include <Wire.h>
//#include <Adafruit_MLX90614.h>

//Adafruit_MLX90614 mlx = Adafruit_MLX90614();
const int trigPin1 = 3;
const int echoPin1 = 4;
//const int trigPin2 = 9;
//const int echoPin2 = 10;
float sum3;
float avg_dist[10] = {0,0,0,0,0,0,0,0,0,0};

float duration, distance;

double rpm_1; 
unsigned long timeold1;
unsigned long timenew_1;
double diff_1;
double old_rpm1;
bool flag;
double sum1;
double avg_rpm[5] = {0,0,0,0,0};
double rpm_2; 
unsigned long timeold2;
unsigned long timenew_2;
double diff_2;
double old_rpm2;
bool flag2;
double sum2;
short prin;
unsigned int final_rpm = 0;
double avg_rpm2[5] = {0,0,0,0,0};

double Average_rpm(double rpm);
void Average_rpm2(double rpm);
float Average_dist(float dist);
void setup() {
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
  rpm_1 = 0; 
  timeold1 = 0;
  timenew_1=0;
  old_rpm1 = 0;
  flag = 0;
  prin =0;
  rpm_2 = 0; 
  timeold2 = 0;
  timenew_2=0;
  old_rpm2 = 0;
  flag2 = 0;
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  //pinMode(trigPin2, OUTPUT);
  //pinMode(echoPin2, INPUT);
  Serial.println("Adafruit MLX90614 test");  
  //mlx.begin();  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!flag){
    if(!digitalRead(10)){
      timenew_1=micros();
      diff_1=timenew_1-timeold1;
      rpm_1=(60000000.0/diff_1);
      timeold1=timenew_1;
      if(rpm_1 < 550 && abs(old_rpm1 - rpm_1) < 150){
       // Serial.println();
      //  Serial.print("1   ");
       // Serial.print(rpm_1);
       // Serial.print("    ");
        Serial.println(Average_rpm(rpm_1));
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



  /*if(mlx.readObjectTempC()<500){
    Serial.print("Temp  ");
    Serial.println(mlx.readObjectTempC());*/
}

double Average_rpm(double rpm){
  avg_rpm[4] = rpm;
  sum1 = 0;
  for(int i =0; i < 5; i++){
    sum1 = sum1 + avg_rpm[i];
  }
  for(int i =0;i<4;i++){
    avg_rpm[i] = avg_rpm[i+1];
  }

  Serial.print("  2:  ");
  Serial.print(final_rpm);
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration = pulseIn(echoPin1, HIGH);
  distance = (duration*.034)/2;
  Serial.print("  Dist1: ");
  Serial.print((distance));

  Serial.print("  1:  ");
  return rpm = sum1/5;
}

void Average_rpm2(double rpm){
  avg_rpm2[4] = rpm;
  sum2 = 0;
  for(int i =0; i < 5; i++){
    sum2 = sum2 + avg_rpm2[i];
  }
  for(int i =0;i<4;i++){
    avg_rpm2[i] = avg_rpm2[i+1];
  }
  rpm = sum2/5;
  if(prin % 8 == 0){
  //  Serial.print("2   ");
    //Serial.print(rpm_2);
    //Serial.print("    ");
// Serial.println(rpm);
    prin = 0;
    final_rpm = rpm;
  }
  prin++;
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
  return dist = sum1/10;
}
