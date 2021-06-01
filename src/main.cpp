#include <Arduino.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <WString.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <PCIManager.h>
#include <PciListenerImp.h>
#include "storedData.h"
#include "stateMgr.h"
#include "manualAutomaticModeState.h"


// Set up the Temperture Sensor
#define TEMPERTURE_SENSOR_PIN 7
// Set up a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(TEMPERTURE_SENSOR_PIN); 
// Pass the oneWire reference to a DallasTemperature component. 
DallasTemperature sensors(&oneWire);

// Set the LCD address to 0x27 and cofigure for a 16 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Set up the Roto Encoder
#define ROTO_ENCODER_CLK_PIN 2
#define ROTO_ENCODER_DT_PIN 3
Encoder rotoEncoder(ROTO_ENCODER_CLK_PIN, ROTO_ENCODER_DT_PIN);

// Set up the Roto Encoder's Push Button Switch
#define ROTO_ENCODER_SW_PIN 4
void OnSwitchPinChange(byte pin, byte pinState);
PciListenerImp switchListener(ROTO_ENCODER_SW_PIN, OnSwitchPinChange);

// Set up the Stored Data Manager (EEPROM Settings)
StoredDataManager storedDataManager{};

// Set up the State Manager
StateManager stateManager{};

// Set up loop to only do work every 1 second
unsigned long lastTime;

/*********************************************************/
void setup()
{
  Serial.begin(9600);

  sensors.begin();   //initialize the temperture sensor

  lcd.init();       //initialize the lcd
  lcd.clear();      //clear the lcd display
  lcd.backlight();  //open the backlight
  
  PciManager.registerListener(&switchListener);

  pinMode(FAN_PWM_PIN, OUTPUT);

  storedDataManager.init();

  stateManager.init(&lcd, &rotoEncoder, &storedDataManager, &sensors);

  lastTime = millis() - 1000;
}
/*********************************************************/
void loop() 
{
  long currentTime = millis();
  if ((currentTime - lastTime) > 1000)
  {
    stateManager.Activity();
    lastTime = currentTime;
  }
}
/************************************************************/

void OnSwitchPinChange(byte pin, byte pinState)
{
  Serial.print(F("Encoder Switch: Pin "));
  Serial.print(pin);
  Serial.print(F(" State: "));
  Serial.println(pinState);
  bool currentEncoderSwitchState = (pinState == 0);
  if (!currentEncoderSwitchState)
  {
    noInterrupts();
    stateManager.Trigger();
    interrupts();
  }
}
