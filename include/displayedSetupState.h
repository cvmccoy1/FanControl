#pragma once

#include "baseSetupModeState.h"

class DisplayedSetupState : public BaseSetupModeState
{
    public:
        DisplayedSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseSetupModeState(modeSetup, lcd, storedDataManager, false, true)
        {
        }

    protected:
        void display() override
        {
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("Display RPM: %3s"), _encoderValue == 0 ? "No" : "Yes");
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getIsRpmsDisplayed();
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setIsRpmsDisplayed(_encoderValue == 0 ? false : true);
        }
};