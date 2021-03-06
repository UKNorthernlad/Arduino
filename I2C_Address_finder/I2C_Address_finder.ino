

#include <Wire.h>

void setup()
{
  // PIN SETUP
  pinMode(A4, INPUT);      // define as input for now, will get redifined by I2C functions
  pinMode(A5, INPUT);      // define as input for now, will get redifined by I2C functions
   
  Serial.begin(9600); 
  printTimestamp();
  Serial.println("I2C Address Finder Running");
  
  Wire.begin();
  
  byte endTXreturnedValue, loopCounter = 0;
  
  for(loopCounter=0; loopCounter<=127; loopCounter++)
  {
    printTimestamp();
    Serial.print("Starting I2C write to address ");
    Serial.println(loopCounter, HEX);

    Wire.beginTransmission(loopCounter);

    printTimestamp();

    endTXreturnedValue = Wire.endTransmission();
    
    if (endTXreturnedValue == 0)
    {
      Serial.print("Correct address is = ");
      Serial.println(loopCounter, HEX);
    }
    
  }
    
}

void loop()
{
  
  // nothing in here, runs only once
    
}

void printTimestamp (void)
{
  Serial.print("[");
  Serial.print(millis());
  Serial.print("]");
}



