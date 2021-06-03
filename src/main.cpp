#include <Arduino.h>
#include <WString.h>
#include <LiquidCrystal_I2C.h>
#include <PCIManager.h>
#include <PciListenerImp.h>
#include "main.h"
#include "storedData.h"
#include "stateMgr.h"

// Set the LCD address to 0x27 and cofigure for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Set up the Roto Encoder's Push Button Switch
#define MODE_TOGGLE_SWITCH_PIN 4
void OnSwitchPinChange(byte pin, byte pinState);
PciListenerImp switchListener(MODE_TOGGLE_SWITCH_PIN, OnSwitchPinChange);

// Set up the Stored Data Manager (EEPROM Settings)
StoredDataManager storedDataManager{};

// Set up the State Manager
StateManager stateManager{};

// Do activity only every 1000 milliseconds (1 second)
const unsigned long ACTIVITY_INTERVAL = 1000;   
unsigned long lastTime;

volatile bool isTriggered = false;

/*********************************************************/
void setup()
{
  Serial.begin(9600);

  lcd.init();       //initialize the lcd
  lcd.clear();      //clear the lcd display
  lcd.backlight();  //open the backlight
  
  PciManager.registerListener(&switchListener);

  storedDataManager.init();
  
  stateManager.init(&lcd, &storedDataManager);

  lastTime = millis() - ACTIVITY_INTERVAL;
}

/*********************************************************/
void loop() 
{
  long currentTime = millis();
  if ((currentTime - lastTime) > ACTIVITY_INTERVAL || isTriggered)
  {
    stateManager.Activity();
    lastTime = currentTime;
    noInterrupts();
    isTriggered = false;
    interrupts();
  }
}

/************************************************************/
void OnSwitchPinChange(byte pin, byte pinState)
{
  bool currentEncoderSwitchState = (pinState == 0);
  if (!currentEncoderSwitchState)
  {
    noInterrupts();
    stateManager.Trigger();
    isTriggered = true;
    interrupts();
  }
}
