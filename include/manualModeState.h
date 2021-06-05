#pragma once

#include "baseNormalModeState.h"

class ManualModeState : public BaseNormalModeState
{
    public:
        ManualModeState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseNormalModeState(manualMode, lcd, storedDataManager)
        {
            _modeName = "Manual";
        }

    protected:
        void enter() override
        {
            BaseNormalModeState::enter();
            _encoderValue = _storedDataManager->getManualModeFanSpeed();
            _encoderAdjustment = 2;
        }
        void leave() override
        {
            BaseNormalModeState::leave();
            _storedDataManager->setManualModeFanSpeed(_encoderValue);
        }
        void GetFanSpeedPWM() override
        {
            if (_temperature >= _storedDataManager->getDesiredTemperature())
            {
                _fanSpeedPWM = _encoderValue;
            }
            else
            {
                _fanSpeedPWM = 0;
            }
        }
};