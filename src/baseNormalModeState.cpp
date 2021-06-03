#include "baseNormalModeState.h"

// Set up the Cooling Fan's Tachometer
#define BASE_FAN_TACH_PIN 8

// The fan provides two interrupts (counts) per revolution, so we need to account for that in our RPM calculations
#define COUNTS_PER_REVOLUTION 2

// To get a more accurate RPM (and to not be changing the LCD display so often), we only do the RPM calculations
// every so often.
#define UPDATE_INTERVAL 2000  // in milliseconds

// Static variable definitions
int BaseNormalModeState::_tachCounter[NUMBER_OF_FANS];

// Public constructor
BaseNormalModeState::BaseNormalModeState(State state, LiquidCrystal_I2C *lcd, Encoder *encoder, StoredDataManager *storedDataManager, DallasTemperature *sensors) : 
    BaseState(state, lcd, encoder, storedDataManager, 0, 255)
{
    _sensors = sensors;
    _calcInterval = UPDATE_INTERVAL / WORK_TIME;
    _calcCounter = _calcInterval;
    _displayState = temperature;
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
    // There are two pulses per revolution, resulting in 4 interrupts per pulse...one on each 
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
    _sensors->requestTemperatures(); 
    _temperature = (int)_sensors->getTempFByIndex(0);
}

void BaseNormalModeState::GetFanSpeedPWM()
{
    _fanSpeedPWM = _encoderValue;
}

// Protected members
void BaseNormalModeState::display(bool firstTime)
{
    int fanSpeedRPM = map(_fanSpeedPWM, 0, 255, 0, 100);
    char fanDisplay[5] = " Off";
    if (fanSpeedRPM > 0)
    {
        snprintf_P(fanDisplay, sizeof(fanDisplay), PSTR("%3d%%"), fanSpeedRPM);
    }
    snprintf_P(_line[0], LCD_COLUMNS+1, PSTR("Fans:%4s %6s"), fanDisplay, _modeName);
    if (--_calcCounter <= 0)
    {
        if (_displayState == temperature)
        {
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("Temperature:%3d\337"), _temperature);
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
    BaseState::display(firstTime);
} 

void BaseNormalModeState::updateValues(bool firstTime)
{
    BaseState::updateValues(firstTime);
    if (firstTime == true)
    {
        _displayState = temperature;
        _calcCounter = 1;
    }
    GetCurrentTemperature();
    GetFanSpeedPWM();
    analogWrite(FAN_PWM_PIN, _fanSpeedPWM);
}

void BaseNormalModeState::retrieveState()
{

}

void BaseNormalModeState::saveState()
{
    for (int fanNumber = 0; fanNumber < NUMBER_OF_FANS; fanNumber++)
    {
        PciListenerImp* listener = _listener[fanNumber];
        PciManager.removeListener(listener);
    }
    BaseState::saveState();
}

void BaseNormalModeState::restoreState()
{
    for (int fanNumber = 0; fanNumber < NUMBER_OF_FANS; fanNumber++)
    {
        byte fanTachPin = BASE_FAN_TACH_PIN + fanNumber;
        PciListenerImp* listener = new PciListenerImp(fanTachPin, OnFanTachPinChange, true);
        PciManager.registerListener(listener);
        _listener[fanNumber] = listener;
        _tachCounter[fanNumber] = 0;
    }
    BaseState::restoreState();
}