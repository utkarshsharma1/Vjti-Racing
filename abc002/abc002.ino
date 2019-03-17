float sum3;
float avg_dist[10] = {0,0,0,0,0,0,0,0,0,0};
float duration, distance;

double rpm_1 = 0, final_rpm_2 = 0, final_rpm_1 = 0; 
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
//short prin;
double final_rpm = 0;
double avg_rpm2[10] = {0,0,0,0,0, 0,0,0,0,0};

void Average_rpm(double rpm);
double Average_rpm2(double rpm);
float Average_dist(float dist);
void setup(void){
  pinMode(7,INPUT_PULLUP);   //Primary 
  pinMode(10,INPUT_PULLUP);      //secondary
  rpm_1 = 0; 
  timeold1 = 0;
  timenew_1=0;
  old_rpm1 = 0;
  flag = 0;
  rpm_2 = 0; 
  timeold2 = 0;
  timenew_2=0;
  old_rpm2 = 0;
  flag2 = 0;
  Serial.begin(9600);
  while(!Serial){
    ;
  }
}

void loop(void) {
  // put your main code here, to run repeatedly:
  if(!flag){
    if(!digitalRead(10)){         // shaft connected to pin 3 of circuit board
      timenew_1 = micros();
      diff_1=timenew_1-timeold1;
      rpm_1=(60000000.0/diff_1);
      timeold1=timenew_1;
      //Serial.println("DGBBG");
     // if(rpm_1 < 550 && abs(old_rpm1 - rpm_1) < 150){
        //Average_rpm(rpm_1);
        Serial.println(rpm_1);
      //}
      old_rpm1 = rpm_1;
      flag = 1;
    }
  }
  if(flag){
    if(digitalRead(10)){
      flag = 0;
    }
  }

//  if(!flag2){
//    if(!digitalRead(7)){        // primary connected to pin 7 of circuit board
//      timenew_2=micros();
//      diff_2=timenew_2-timeold2;
//      rpm_2=(60000000.0/diff_2);
//      timeold2=timenew_2;
//      if(rpm_2 < 4400 && rpm_2 > 1400 && (abs(old_rpm2 - rpm_2) < 1000)){
//        final_rpm_2 = Average_rpm2(rpm_2);
//         Serial.println(rpm_2);
//      }
//      old_rpm2 = rpm_2;
//      flag2 = 1;
//    }
//  }
//  if(flag2){
//    if(digitalRead(7)){
//      flag2 = 0;
//    }
//  }
}

void Average_rpm(double rpm_1){
  avg_rpm[4] = rpm_1;
  sum1 = 0;
  for(int i =0; i < 5; i++){
    sum1 = sum1 + avg_rpm[i];
  }
  for(int i =0;i<4;i++){
    avg_rpm[i] = avg_rpm[i+1];
  }
  rpm_1 = sum1/5;
  final_rpm_1 = rpm_1;
  
  Serial.print("  1:  ");
  Serial.print(final_rpm_1);
  
  Serial.print("  2: ");
  Serial.println(final_rpm_2);
}

double Average_rpm2(double rpm_2){
  avg_rpm2[9] = rpm_2;
  sum2 = 0;
  for(int i =0; i < 10; i++){
    sum2 = sum2 + avg_rpm2[i];
  }
  for(int i =0; i< 9; i++){
    avg_rpm2[i] = avg_rpm2[i+1];
  }
  return (sum2/10);
}
