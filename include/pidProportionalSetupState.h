#pragma once

#include "baseSetupModeState.h"

class PidProportionSetupState : public BaseSetupModeState
{
    public:
        PidProportionSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseSetupModeState(pidProportionalSetup, lcd, storedDataManager, PID_PROPORTIONAL_MIN, PID_PROPORTIONAL_MAX)
        {
        }

    protected:
        void display() override
        {
            int x = _encoderValue / 10;
            int y = _encoderValue % 10;
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("  PID kp: %2d.%1d  "), x, y);
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getPidProportional();
            _encoderAdjustment = 2;
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setPidProportional(_encoderValue);
        }
};