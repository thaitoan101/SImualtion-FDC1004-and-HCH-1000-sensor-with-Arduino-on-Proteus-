#include <Wire.h>
#include <Protocentral_FDC1004.h>

int capdac = 0;

FDC1004 fdc;

void setup() {
  //Wire.begin();
  Serial.begin(9600);
}

void loop() {
  uint8_t measurement = 0;
  uint8_t channel = 0;
  
  char result[100];


  fdc.configureMeasurementSingle(measurement, channel, capdac);
  fdc.triggerSingleMeasurement(measurement, FDC1004_100HZ);
  //wait for completion
  delay(15);
  uint16_t value[2];
  if (! fdc.readMeasurement(measurement, value)) {
    
    // calculate capacitance;
    // The absolute capacitance is a function of the capdac and the measurement
    // We only use the msb because the FDC1004 only has 16bits effective resolution;
    // the last 8 bits are more or less random noise. 
    int16_t msb = (int16_t) value[0];
    int32_t capacitance = ((int32_t)457) * ((int32_t)msb); //in attofarads
    capacitance /= 1000; //in femtofarads
    capacitance += ((int32_t)3028) * ((int32_t)capdac);

    

      
    Serial.print("The relative humidity is : ");
    Serial.print(capacitance*1.718-506.625);
    Serial.print("\r\n");
    //Serial.print(" fF\n");
    delay(300);
    
    //adjust capdac
    int16_t upper_bound = 0x4000;
    int16_t lower_bound = -1 * upper_bound;
    if (msb > upper_bound) {
      if (capdac < FDC1004_CAPDAC_MAX) capdac++;
    } else if (msb < lower_bound) {
      if (capdac > 0) capdac--;
    }
  }
  delay(200);
}
