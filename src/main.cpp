#include <Arduino.h>
#include <WString.h>
#include <LiquidCrystal_I2C.h>
#include <PCIManager.h>
#include "myPciListenerImp.h"
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
MyPciListenerImp switchListener(MODE_TOGGLE_SWITCH_PIN, OnSwitchPinChange, true);

// The Stored Data Manager (EEPROM Settings)
StoredDataManager* storedDataManager = nullptr;

// The State Manager
StateManager* stateManager;

#define FAST_ACTIVITY_LOOP_INTERVAL  200
#define SLOW_ACTIVITY_LOOP_INTERVAL  1000  
unsigned long lastTime;
volatile bool hasStateTriggered = false;

unsigned long GetActivityInterval()
{
  if (storedDataManager != nullptr)
    return storedDataManager->getIsRpmsDisplayed() ? SLOW_ACTIVITY_LOOP_INTERVAL : FAST_ACTIVITY_LOOP_INTERVAL;
  else
    return FAST_ACTIVITY_LOOP_INTERVAL;
}

bool inline HasActivityIntervalElapsed(long currentTime)
{   
  return ((currentTime - lastTime) > GetActivityInterval());
}

/*********************************************************/
void setup()
{
  //Serial.begin(9600);

  lcd.init();       //initialize the lcd
  lcd.clear();      //clear the lcd display
  lcd.backlight();  //open the backlight
  
  PciManager.registerListener(&switchListener);
  storedDataManager = new StoredDataManager();
  stateManager = new StateManager(&lcd, storedDataManager);
  lastTime = millis() - GetActivityInterval();
}

/*********************************************************/
void loop() 
{
  long currentTime = millis();
  if (hasStateTriggered || HasActivityIntervalElapsed(currentTime))
  {
    stateManager->Activity();
    noInterrupts();
    hasStateTriggered = false;
    interrupts();
    lastTime = currentTime;
  }
}


/************************************************************/
void OnSwitchPinChange(byte pin, byte pinState)
{
  // Make sure the encoder switch has constantly been in the low state (button pressed) 
  // for at least the DEBOUCE_INTERVAL. Otherwise, it is not considered a value button press.
  bool isLowState = (pinState == CHANGEKIND_HIGH_TO_LOW);
  if (isLowState)
  {
    // Falling Edge Trigger...record the time of the falling edge
    noInterrupts();
    fallingEdgeTime = millis();
    interrupts();
  }
  else
  {
    // Raisting Edge Trigger
    noInterrupts();
    if (fallingEdgeTime != 0)  // Make sure we have already seen the falling edge
    {
      unsigned long timeSinceFallingEdge = millis() - fallingEdgeTime;
      if (timeSinceFallingEdge >= DEBOUCE_INTERVAL)
      {
        // Encoder Switch was low for at least the DEBOUCE_INTERVAL...OK to trigger to next state
        stateManager->Trigger();
        hasStateTriggered = true;
      }
      fallingEdgeTime = 0;
    }
    interrupts();
  }
}
