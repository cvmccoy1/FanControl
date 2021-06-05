#pragma once

#include "baseSetupModeState.h"

class DesiredTempSetupState : public BaseSetupModeState
{
    public:
        DesiredTempSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseSetupModeState(modeSetup, lcd, storedDataManager, DESIRED_TEMP_MIN, DESIRED_TEMP_MAX)
        {
        }

    protected:
        void display() override
        {
            snprintf_P(_line[1], LCD_COLUMNS+1, PSTR(" Fans On At %3d\337"), _encoderValue);
            BaseSetupModeState::display();
        }
        void enter() override
        {
            BaseSetupModeState::enter();
            _encoderValue = _storedDataManager->getDesiredTemperature();
        }
        void leave() override
        {
            BaseSetupModeState::leave();
            _storedDataManager->setDesiredTemperature(_encoderValue);
        }};