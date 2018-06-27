double rpm_1; 
unsigned long timeold1;
unsigned long timenew_1;
double diff_1;
double old_rpm1;
bool flag;
double sum;
double avg_rpm[5] = {0,0,0,0,0};
double rpm_2; 
unsigned long timeold2;
unsigned long timenew_2;
double diff_2;
double old_rpm2;
bool flag2;
double sum2;
short prin;
double avg_rpm2[5] = {0,0,0,0,0};

double Average_rpm(double rpm);
void Average_rpm2(double rpm);
void setup() {
  pinMode(3,INPUT_PULLUP);   //Primary 
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!flag){
    if(!digitalRead(3)){
      timenew_1=micros();
      diff_1=timenew_1-timeold1;
      rpm_1=(60000000.0/diff_1);
      timeold1=timenew_1;
      if(rpm_1 < 550 && abs(old_rpm1 - rpm_1) < 150){
        Serial.print("1   ");
        Serial.print(rpm_1);
        Serial.print("    ");
        Serial.println(Average_rpm(rpm_1));
      }
      old_rpm1 = rpm_1;
      flag = 1;
    }
  }
  if(flag){
    if(digitalRead(3)){
      flag = 0;
    }
  }

  if(!flag2){
    if(!digitalRead(4)){
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
    if(digitalRead(4)){
      flag2 = 0;
    }
  }
}

double Average_rpm(double rpm){
  avg_rpm[4] = rpm;
  sum = 0;
  for(int i =0; i < 5; i++){
    sum = sum + avg_rpm[i];
  }
  for(int i =0;i<4;i++){
    avg_rpm[i] = avg_rpm[i+1];
  }
  return rpm = sum/5;
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
    Serial.print("2   ");
    Serial.print(rpm_2);
    Serial.print("    ");
    Serial.println(rpm);
    prin = 0;
  }
  prin++;
}

