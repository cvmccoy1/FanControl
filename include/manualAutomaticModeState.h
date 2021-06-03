#pragma once

#include <DallasTemperature.h>
#include <PCIManager.h>
#include <PciListenerImp.h>
#include "main.h"
#include "baseState.h"

#define NUMBER_OF_FANS 2
// Set up the Cooling Fan PWM
#define FAN_PWM_PIN 6
  

// Set up the Cooling Fan's Tachometer
#define BASE_FAN_TACH_PIN 8

// The fan provides two interrupts (counts) per revolution, so we need to account for that in our RPM calculations
#define COUNTS_PER_REVOLUTION 2

// To get a more accurate RPM (and to not be changing the LCD display so often), we only do the RPM calculations
// every so often.
#define UPDATE_INTERVAL 2000  // in milliseconds

enum DisplayState { temperature, rpms, numberOfDisplayStates };

class ManualAutomaticModeState : BaseState
{
    private:
        bool _isAutomaticMode;
        DallasTemperature *_sensors;
        static int _tachCounter[NUMBER_OF_FANS];
        PciListenerImp* _listener[NUMBER_OF_FANS];
        const int _calcInterval = UPDATE_INTERVAL / WORK_TIME;
        int _calcCounter = _calcInterval;
        DisplayState _displayState = temperature;

        unsigned long calcRPM(byte index)
        {
            noInterrupts();
            unsigned long tachCounter = _tachCounter[index];
            _tachCounter[index] = 0;
            interrupts();
            unsigned long rpm = (double)tachCounter * 60.0 / (double)(COUNTS_PER_REVOLUTION * _calcInterval * numberOfDisplayStates);
            return rpm;
        }
        static void OnFanTachPinChange(byte fanPin, byte pinState)
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
        void GetCurrentTemperture()
        {
            _sensors->requestTemperatures(); 
            _temperture = (int)_sensors->getTempFByIndex(0);
        }
        virtual void GetFanSpeedPWM()
        {
            _fanSpeedPWM = _encoderValue;
        }
    protected:
        int _temperture;
        int _fanSpeedPWM;
    public:
        ManualAutomaticModeState(bool isAutomaticMode, LiquidCrystal_I2C *lcd, Encoder *encoder, StoredDataManager *storedDataManager, DallasTemperature *sensors) : 
            BaseState(isAutomaticMode ? automaticMode : manualMode, lcd, encoder, storedDataManager, 0, 255)
        {
            _isAutomaticMode = isAutomaticMode;
            _sensors = sensors;  
        }
    protected:
        void display(bool firstTime) override
        {
            int fanSpeedRPM = map(_fanSpeedPWM, 0, 255, 0, 100);
            snprintf_P(_line[0], LCD_COLUMNS+1, PSTR("Fan Speed: %3d%% "), fanSpeedRPM);
            if (--_calcCounter <= 0)
            {
                if (_displayState == temperature)
                {
                    snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("Temp:%3d\337 %6s"), _temperture, _isAutomaticMode ? "Auto" : "Manual");
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
        void updateValues(bool firstTime) override
        {
            BaseState::updateValues(firstTime);
            if (firstTime == true)
            {
                _displayState = temperature;
                _calcCounter = 1;
            }
            GetCurrentTemperture();
            GetFanSpeedPWM();
            analogWrite(FAN_PWM_PIN, _fanSpeedPWM);
        }
        void retrieveState() override
        {

        }
        void saveState() override
        {
            for (int fanNumber = 0; fanNumber < NUMBER_OF_FANS; fanNumber++)
            {
                PciListenerImp* listener = _listener[fanNumber];
                PciManager.removeListener(listener);
            }
            if (!_isAutomaticMode && _storedDataManager->getManualModeFanSpeed() != _encoderValue)
            {
                _storedDataManager->setManualModeFanSpeed(_encoderValue);
                _storedDataManager->save();
            }
            BaseState::saveState();
        }
        void restoreState() override
        {
            if (!_isAutomaticMode)
            {
                _encoderValue = _storedDataManager->getManualModeFanSpeed();
            }

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
};

int ManualAutomaticModeState::_tachCounter[NUMBER_OF_FANS];