#pragma once

#include "baseNormalModeState.h"

#define STABLE_INTERVAL 60000  // Changes in Fan Speed must be stable for a minute before auto saving in EEPROM
                               //   or turning off the fans if below the desired temperature.

class ManualModeState : public BaseNormalModeState
{
    public:
        ManualModeState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseNormalModeState(manualMode, lcd, storedDataManager)
        {
            _modeName = "Manual";           
        }

    private:
        int32_t _currentEncoderValue;
        unsigned long _startTime;
        bool inline HasEncoderValueChanged() { return _encoderValue != _currentEncoderValue; }
        bool inline IsWaitingForStablization() { return _startTime > 0 ; }
        bool inline HasFanSpeedStablized() { return (millis() - _startTime) >= STABLE_INTERVAL; }
        bool inline HasReachedDesiredTemperature() { return _temperature < _storedDataManager->getDesiredTemperature(); }
        void CheckFanSpeedOffCondition()
        {
            if (HasReachedDesiredTemperature() && _fanSpeedPWM != 0)
            {
                _fanSpeedPWM = 0;
            }
        }

    protected:
        void enter() override
        {
            BaseNormalModeState::enter();
            _encoderValue = _storedDataManager->getManualModeFanSpeed();
            _encoderAdjustment = 2;
            _fanSpeedPWM = _currentEncoderValue = _encoderValue;
            CheckFanSpeedOffCondition();
            _startTime = 0;
        }
        void leave() override
        {
            BaseNormalModeState::leave();
            if (IsWaitingForStablization())
            {
                _storedDataManager->setManualModeFanSpeed(_encoderValue);
            }
        }
        void GetFanSpeedPWM() override
        {
            // If the Manual Fan Speed has changed, wait for it to be stable for 
            // a certain amount of time (STABLE_INTERVAL) before saving it to EEPROM
            if (HasEncoderValueChanged())
            {
                _startTime = millis();
                _currentEncoderValue = _encoderValue;
                _fanSpeedPWM = _encoderValue;
            }
            else if (IsWaitingForStablization())
            {
                if (HasFanSpeedStablized())
                {
                    _storedDataManager->setManualModeFanSpeed(_encoderValue);
                    _storedDataManager->Save();          
                    _startTime = 0;
                    CheckFanSpeedOffCondition();
                }
            }
            else 
            {
                CheckFanSpeedOffCondition();
            }
        }
};