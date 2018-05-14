volatile double rpm_1=0; 
volatile double rpm_2=0; 
volatile unsigned long timeold1=0;
volatile unsigned long timenew_1=0;
volatile unsigned long timeold2=0;
volatile unsigned long timenew_2=0;
volatile double diff_1=0;
volatile double diff_2=0;
volatile int i1 = 0;
volatile int i2 = 0;
volatile float cvt=0;

void setup()
{ 
  //lcd.begin(16,2);
  pinMode(3,INPUT_PULLUP);   //Primary 
  pinMode(2,INPUT_PULLUP);   //Secondary
  attachInterrupt(digitalPinToInterrupt(3), rpm_fun_1,FALLING);  
  attachInterrupt(digitalPinToInterrupt(2), rpm_fun_2,FALLING);  
  rpm_1 = 0;//Driving Pulley 
  rpm_2 = 0;//Driven Pulley 
  timeold1 = 0;
  timenew_1=0;
  timeold2 = 0;
  timenew_2=0;
  Serial.begin(9600);

  //SDCARD_SETUP

//Open serial communications and wait for port to open:
  //Serial.begin(250000);
  /*pinMode(53, OUTPUT);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
Serial.print("Initializing SD card...");
if (!SD.begin(chipSelect)) 
  {
    Serial.println("initialization failed!");
    return;
  }
Serial.println("initialization done.");

}*/
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
  //Serial.println(diff_1);
  }
  if(i1==0)
  { 
    rpm_1=(60000000.0/diff_1);
    timeold1=timenew_1;
    //Serial.println(diff_1);  
  }
  i1++;
  if(i1==1)
  {
    i1=0;
  }
  diff_1 = 0;
}

void rpm_fun_2()
{
  timenew_2=micros();
  if(timenew_2==timeold2)
 { rpm_2=0;}
  else
  {
  diff_2=timenew_2-timeold2;
  //Serial.println(diff_2);
  }
  if(i2==0)
  { 
    rpm_2=(60000000.0/diff_2);
    timeold2=timenew_2;
    //Serial.println(diff_2);
    
  }
  i2++;
  if(i2==3)
  {
    i2=0;
  }
  diff_2 = 0;
}

void loop()
{ 
 // cvt=rpm_1/(rpm_2*11.9025);
 // if(cvt>0.5&&cvt<3.2)
 // {
    Serial.print(rpm_1);
    Serial.print("       ");
 /*   Serial.print(rpm_2);
    Serial.print("       ");
    Serial.print(cvt);
    Serial.print("       ");
    //Serial.println(millis());
   */ delay(100);
    //lcd.clear();
    //lcd.setCursor(0,0);
    //lcd.print(cvt);
    //lcd.setCursor(0,1);
    Serial.println(2*18*3.142*rpm_1*0.254/(5*60));
 // }
  /*else
  {
    lcd.setCursor(0,0);
    lcd.print("VJTI");
    lcd.setCursor(0,1);
    lcd.print("Racing");
    }*/
}
    

/*File Driving = SD.open("rpm1.txt", FILE_WRITE);
// if the file is available, write to it:
if (Driving) 
{
Driving.println(rpm_1);
Driving.close();
} 
// if the file isn't open, pop up an error:
else {
Serial.println("error opening test.txt");
}

File Driven = SD.open("rpm2.txt", FILE_WRITE);
// if the file is available, write to it:
if (Driven) 
{
Driven.println(rpm_2);
Driven.close();
} 
// if the file isn't open, pop up an error:
else {
Serial.println("error opening test.txt");

}

File timing = SD.open("time.txt", FILE_WRITE);
// if the file is available, write to it:
if (timing) 

{
timing.println(millis());
timing.close();
} 
// if the file isn't open, pop up an error:
else {
Serial.println("error opening time.txt");
*/




