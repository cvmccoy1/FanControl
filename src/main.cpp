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
#define DEBOUCE_INTERVAL 50  // In milliseconds
void OnSwitchPinChange(byte pin, byte pinState);
unsigned long fallingEdgeTime = 0;  // Used for debouncing the encoder switch button
PciListenerImp switchListener(MODE_TOGGLE_SWITCH_PIN, OnSwitchPinChange, true);

// Set up the Stored Data Manager (EEPROM Settings)
StoredDataManager storedDataManager{};

// Set up the State Manager
StateManager stateManager(&lcd, &storedDataManager);

unsigned long gActivityInterval;   
unsigned long lastTime;
volatile bool isTriggered = false;

/*********************************************************/
void setup()
{
  //Serial.begin(9600);

  lcd.init();       //initialize the lcd
  lcd.clear();      //clear the lcd display
  lcd.backlight();  //open the backlight
  
  PciManager.registerListener(&switchListener);

  lastTime = millis() - gActivityInterval;
}

/*********************************************************/
void loop() 
{
  gActivityInterval = storedDataManager.getIsRpmsDisplayed() ? 1000 : 200;
  long currentTime = millis();
  if ((currentTime - lastTime) > gActivityInterval || isTriggered)
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
  if (currentEncoderSwitchState)
  {
    noInterrupts();
    fallingEdgeTime = millis();
    interrupts();
  }
  else
  {
    noInterrupts();
    if (fallingEdgeTime != 0)
    {
      unsigned long timeSinceFallingEdge = millis() - fallingEdgeTime;
      if (timeSinceFallingEdge >= DEBOUCE_INTERVAL)
      {
        stateManager.Trigger();
        isTriggered = true;
      }
      fallingEdgeTime = 0;
    }
    interrupts();
  }
}
