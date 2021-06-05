#pragma once

#include "baseSetupModeState.h"

class PidIntegralSetupState : public BaseSetupModeState
{
    public:
        PidIntegralSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseSetupModeState(pidIntegralSetup, lcd, storedDataManager, PID_INTEGRAL_MIN, PID_INTEGRAL_MAX)
        {
        }

    protected:
        void display() override
        {
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("   PID ki: %2d   "), _encoderValue);
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getPidIntegral();
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setPidIntegral(_encoderValue);
        }
};