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
            int x = _encoderValue / 10;
            int y = _encoderValue % 10;
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("  PID ki: %2d.%1d  "), x, y);
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getPidIntegral();
            _encoderAdjustment = 2;
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setPidIntegral(_encoderValue);
        }
};