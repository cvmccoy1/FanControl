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
        void saveState() override
        {
            if (_storedDataManager->getManualModeFanSpeed() != _encoderValue)
            {
                _storedDataManager->setManualModeFanSpeed(_encoderValue);
                _storedDataManager->save();
            }
            BaseNormalModeState::saveState();
        }
        void restoreState() override
        {
            _encoderValue = _storedDataManager->getManualModeFanSpeed();
            BaseNormalModeState::restoreState();
        }
};