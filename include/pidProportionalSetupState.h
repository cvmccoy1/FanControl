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
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("   PID kp: %2d   "), _encoderValue);
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getPidProportional();
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setPidProportional(_encoderValue);
        }
};