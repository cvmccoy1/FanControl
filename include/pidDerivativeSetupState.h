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
            int x = _encoderValue / 10;
            int y = _encoderValue % 10;
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("  PID kd: %2d.%1d  "), x, y);
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getPidDerivative();
            _encoderAdjustment = 2;
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setPidDerivative(_encoderValue);
        }
};