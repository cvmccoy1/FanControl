#include <OneWire.h> 
#include <DallasTemperature.h>
#include "baseNormalModeState.h"

// Set up the Cooling Fan PWM
#define FAN_PWM_PIN 6

// Set up the Cooling Fan's Tachometer
#define BASE_FAN_TACH_PIN 8

// The fan provides two interrupts (counts) per revolution, so we need to account for that in our RPM calculations
#define COUNTS_PER_REVOLUTION 2

// To get a more accurate RPM (and to not be changing the LCD display so often), we only do the RPM calculations
// every so often.
#define UPDATE_INTERVAL 2000  // in milliseconds


// Set up the Temperture Sensor
#define TEMPERTURE_SENSOR_PIN 7
// Set up a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(TEMPERTURE_SENSOR_PIN); 
// Pass the oneWire reference to a DallasTemperature component. 
DallasTemperature sensors(&oneWire);

// Static variable definitions
int BaseNormalModeState::_tachCounter[NUMBER_OF_FANS];

// Public constructor
BaseNormalModeState::BaseNormalModeState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) : 
    BaseState(state, lcd, storedDataManager, 0, 255)
{
    _calcInterval = UPDATE_INTERVAL / ACTIVITY_INTERVAL;
    _calcCounter = _calcInterval;
    _displayState = temperature;
    pinMode(FAN_PWM_PIN, OUTPUT);
    sensors.begin();   //initialize the temperture sensor
}
 
// Private members
unsigned long BaseNormalModeState::calcRPM(byte index)
{
    noInterrupts();
    unsigned long tachCounter = _tachCounter[index];
    _tachCounter[index] = 0;
    interrupts();
    unsigned long rpm = (double)tachCounter * 60.0 / (double)(COUNTS_PER_REVOLUTION * _calcInterval * numberOfDisplayStates);
    return rpm;
}

void BaseNormalModeState::OnFanTachPinChange(byte fanPin, byte pinState)
{
    // There are two pulses per revolution, resulting in 4 interrupts per revolution...one on each 
    // raising and trailing edge. We are only going to count the raising edge transitions.
    if (pinState != 0)
    {
        byte index = fanPin - BASE_FAN_TACH_PIN;
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
    int fanSpeedRPM = map(_fanSpeedPWM, 0, 255, 0, 100);
    char fanDisplay[5] = " Off";
    if (fanSpeedRPM > 0)
    {
        snprintf_P(fanDisplay, sizeof(fanDisplay), PSTR("%3d%%"), fanSpeedRPM);
    }
    snprintf_P(_line[0], LCD_COLUMNS+1, PSTR("%6s Fans:%4s"), _modeName, fanDisplay);
    if (--_calcCounter <= 0)
    {
        if (_displayState == temperature)
        {
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("Temperature:%3d\337"), _temperature);
            if (_isRpmsDisplayed)
                _displayState = rpms;
        }
        else if (_displayState == rpms)
        {
            int rpm1 = calcRPM(0);
            int rpm2 = calcRPM(1);
            if (fanSpeedRPM > 0)
            {
                snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("Rpm1:%4d 2:%4d"), rpm1, rpm2);
            }
            _displayState = temperature;
        }
        _calcCounter = _calcInterval;
    }
    BaseState::display();
} 

void BaseNormalModeState::updateValues()
{
    BaseState::updateValues();
    GetCurrentTemperature();
    GetFanSpeedPWM();
    analogWrite(FAN_PWM_PIN, _fanSpeedPWM);
}

void BaseNormalModeState::enter()
{
    BaseState::enter();
    _isRpmsDisplayed = _storedDataManager->getIsRpmsDisplayed();
    _displayState = temperature;
    _calcCounter = 1;
    if (_isRpmsDisplayed)
    {
        for (int fanNumber = 0; fanNumber < NUMBER_OF_FANS; fanNumber++)
        {
            byte fanTachPin = BASE_FAN_TACH_PIN + fanNumber;
            PciListenerImp* listener = new PciListenerImp(fanTachPin, OnFanTachPinChange, true);
            PciManager.registerListener(listener);
            _listener[fanNumber] = listener;
            _tachCounter[fanNumber] = 0;
        }
    }
}

void BaseNormalModeState::leave()
{
    BaseState::leave();
    if (_isRpmsDisplayed)
    {
        for (int fanNumber = 0; fanNumber < NUMBER_OF_FANS; fanNumber++)
        {
            PciListenerImp* listener = _listener[fanNumber];
            PciManager.removeListener(listener);
        }
    }
}