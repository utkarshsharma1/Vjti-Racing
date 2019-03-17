#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
const int trigPin = 3;
const int echoPin = 4;
float sum;
float avg_dist[5] = {0,0,0,0,0};

float duration, distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test");  
  mlx.begin();  
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.034)/2;
  Serial.print("Distance: ");
  Serial.println(Average_dist(distance));

  if(mlx.readObjectTempC()<500){
    Serial.println(mlx.readObjectTempC());
  }
   
  delay(1000);
}

float Average_dist(float dist){
  avg_dist[4] = dist;
  sum = 0;
  for(int i =0; i < 5; i++){
    sum = sum + avg_dist[i];
  }
  for(int i =0;i<4;i++){
    avg_dist[i] = avg_dist[i+1];
  }
  return dist = sum/5;
}
