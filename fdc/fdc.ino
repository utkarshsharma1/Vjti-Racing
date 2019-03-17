#include <Wire.h>
#include <Protocentral_FDC1004.h>

#define UPPER_BOUND  0X4000                 // max readout capacitance
#define LOWER_BOUND  (-1 * UPPER_BOUND)
#define CHANNEL 0                          // channel to be read
#define MEASURMENT 0                       // measurment channel

const int chipSelect = 10;
int capdac = 0;
float avg_capaci[50] = {0};
float capaci = 0, final_capaci = 0, sum;
//char result[100];

FDC1004 FDC;

void Average_capaci(float capaci);

void setup()
{
  Wire.begin();        //i2c begin
  Serial.begin(9600); // serial baud rate
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
 /* if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);  
 
  Serial.println("card initialized.");*/
}

void loop()
{
 // String dataString = "";
  
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
    Average_capaci(capaci);
    Serial.println(capaci);
   
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
 // File dataFile = SD.open("datalee2.txt", FILE_WRITE);

  // if the file is available, write to it:
 /* if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }*/
  delay(150);
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
