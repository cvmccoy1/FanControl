#include <OneWire.h> 
#include <DallasTemperature.h>
#include "baseNormalModeState.h"


// The fan provides two interrupts (counts) per revolution, so we need to account for that in our RPM calculations
#define COUNTS_PER_REVOLUTION 2

// To get a more accurate RPM (and to not be changing the LCD display so often), we only do the RPM calculations
// every so often.
#define UPDATE_INTERVAL 3000  // in milliseconds

// Set up the Temperture Sensor
#define TEMPERATURE_SENSOR_PIN 7
// Set up a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
// Pass the oneWire reference to a DallasTemperature component. 
DallasTemperature sensors(&oneWire);

// Specify which PWM pins to use (on the nano, 3, 5, 6, 9, 10, and 11 are available)
#define FAN1_PWM_PIN 5
#define FAN2_PWM_PIN 6
// Specify which input pins will act as the Cooling Fans' Tachometer
#define FAN1_TACH_PIN 8
#define FAN2_TACH_PIN 9

const byte FAN_PWM_PIN[NUMBER_OF_FANS] = { FAN1_PWM_PIN, FAN2_PWM_PIN };
const byte FAN_TACH_PIN[NUMBER_OF_FANS] = { FAN1_TACH_PIN, FAN2_TACH_PIN };

// Static variable definitions
int BaseNormalModeState::_tachCounter[NUMBER_OF_FANS];

// Public constructor
BaseNormalModeState::BaseNormalModeState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) : 
    BaseState(state, lcd, storedDataManager, 0, 255)
{
    for (int fanNumberIndex = 0; fanNumberIndex < NUMBER_OF_FANS; fanNumberIndex++)
    {
        pinMode(FAN_PWM_PIN[fanNumberIndex], OUTPUT);
    }
    sensors.begin();   //initialize the temperture sensor
}
 
// Private members
unsigned long BaseNormalModeState::calcRPM(byte index)
{
    noInterrupts();
    unsigned long tachCounter = _tachCounter[index];
    _tachCounter[index] = 0;
    interrupts();
    unsigned long rpm = (double)tachCounter * 60.0 / (double)(COUNTS_PER_REVOLUTION * _numberOfCyclesToDisplay * numberOfDisplayStates);
    return rpm;
}

byte BaseNormalModeState::FindIndexByFanTachPin(byte fanTachPin)
{
    int index = 0;
    while (index < NUMBER_OF_FANS)
    {
        if (FAN_TACH_PIN[index] == fanTachPin) {
            break;
        }
        index++;
    }
    return index;
}

void BaseNormalModeState::OnFanTachPinChange(byte fanTachPin, byte pinState)
{
    // There are two pulses per revolution, resulting in 4 interrupts per revolution...one on each 
    // raising and trailing edge. We are only going to count the raising edge transitions.
    if (pinState == CHANGEKIND_LOW_TO_HIGH)
    {

        byte index = FindIndexByFanTachPin(fanTachPin);
        noInterrupts();
        _tachCounter[index]++;
        interrupts();
    }
}

void BaseNormalModeState::GetCurrentTemperature()
{
    sensors.requestTemperatures(); 
    _temperature = (int)sensors.getTempFByIndex(0);
}

// Protected override members
void BaseNormalModeState::display()
{
    // Display the Fan Speed on every Display cycle
    int fanSpeedRPM = map(_fanSpeedPWM, 0, 255, 0, 100);
    char fanDisplay[5] = " Off";
    if (fanSpeedRPM > 0)
    {
        snprintf_P(fanDisplay, sizeof(fanDisplay), PSTR("%3d%%"), fanSpeedRPM);
    }
    snprintf_P(_line[0], LCD_COLUMNS+1, PSTR("%6s Fans:%4s"), _modeName, fanDisplay);
    _displayCycleCounter--;

    // The Temperature and FAN RPMs alternate every _numberOfCyclesToDisplay cycles (typically about 4 cycles).  
    // if the DisplayRPMs setting is false or the FanSpeedPWM is zero, the FAN RPMs are not displays
    // and the Temperature will always be displayed.  When the temperature is to be displayed the
    // current temperature is displayed on every cycle.  When the FAN RPMs are to be displayed,
    // the rpms are only calculated and displayed on the first cycle...this allows for a more accurate reading of the RPMs.
    if (_displayState == temperature)
    {
        if (_displayCycleCounter <= 0)
        {
            if (_isRpmsDisplayed && fanSpeedRPM > 0) 
            {
                // Change to start displaying the Fan RPMs
                _displayState = rpms;
                // TODO: Change the following code so that it can handle displaying more than two fan speeds
                int rpm1 = calcRPM(0);
                int rpm2 = calcRPM(1);
                snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("Rpm1:%4d 2:%4d"), rpm1, rpm2);
            }
            _displayCycleCounter = _numberOfCyclesToDisplay;  // Reset the Display Cycle counter
        }
    }
    else if (_displayState == rpms)  
    {
        if (_displayCycleCounter <= 0 || fanSpeedRPM == 0)  // if exit RPMs display state
        {
            // Change to start displaying the Temperature...gets displayed below
            _displayState = temperature;
            _displayCycleCounter = _numberOfCyclesToDisplay;  // Reset the Display Cycle counter
        }
    }
    if (_displayState == temperature)
    {
        // Display the temperature on all cycles while in the Temperature Display State
        snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("Temperature:%3d\337"), _temperature);
    }
    BaseState::display();
} 

void BaseNormalModeState::updateValues()
{
    BaseState::updateValues();
    GetCurrentTemperature();
    GetFanSpeedPWM();
    for (int fanNumberIndex = 0; fanNumberIndex < NUMBER_OF_FANS; fanNumberIndex++)
    {
        analogWrite(FAN_PWM_PIN[fanNumberIndex], _fanSpeedPWM);
    }
}

void BaseNormalModeState::enter()
{
    BaseState::enter();
    _numberOfCyclesToDisplay = UPDATE_INTERVAL / GetActivityInterval();
    _displayCycleCounter = _numberOfCyclesToDisplay;
    _isRpmsDisplayed = _storedDataManager->getIsRpmsDisplayed();
    _displayState = temperature;
    _displayCycleCounter = _numberOfCyclesToDisplay + 1;
    if (_isRpmsDisplayed)
    {
        for (int fanNumberIndex = 0; fanNumberIndex < NUMBER_OF_FANS; fanNumberIndex++)
        {
            byte fanTachPin = FAN_TACH_PIN[fanNumberIndex];
            MyPciListenerImp* listener = new MyPciListenerImp(fanTachPin, OnFanTachPinChange, true);
            PciManager.registerListener(listener);
            _listener[fanNumberIndex] = listener;
            _tachCounter[fanNumberIndex] = 0;
        }
    }
}

void BaseNormalModeState::leave()
{
    BaseState::leave();
    if (_isRpmsDisplayed)
    {
        for (int fanNumberIndex = 0; fanNumberIndex < NUMBER_OF_FANS; fanNumberIndex++)
        {
            MyPciListenerImp* listener = _listener[fanNumberIndex];
            PciManager.removeListener(listener);
        }
    }
}