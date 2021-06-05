#pragma once

#include "baseSetupModeState.h"

class ModeSetupState : public BaseSetupModeState
{
    public:
        ModeSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseSetupModeState(modeSetup, lcd, storedDataManager, manual, automatic)
        {
        }

    protected:
        void display() override
        {
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("  Mode: %6s  "), _encoderValue == 0 ? "Manual" : "Auto");
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getMode();    
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setMode(_encoderValue == 0 ? manual : automatic);
        }
};