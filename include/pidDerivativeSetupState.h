#pragma once

#include "baseSetupModeState.h"

class PidDerivativeSetupState : public BaseSetupModeState
{
    public:
        PidDerivativeSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseSetupModeState(pidDerivativeSetup, lcd, storedDataManager, PID_DERIVATIVE_MIN, PID_DERIVATIVE_MAX)
        {
        }

    protected:
        void display() override
        {
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("   PID kd: %2d   "), _encoderValue);
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getPidDerivative();
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setPidDerivative(_encoderValue);
        }
};