#pragma once

#include "baseSetupModeState.h"

class BasePidSetupState : public BaseSetupModeState
{
    private:
        char _symbol;
        int (StoredDataManager::*_getValue) ();
        void (StoredDataManager::*_setValue) (int);

    public:
        BasePidSetupState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager, int32_t encoderMinValue, int32_t encoderMaxValue);

    protected:
        void display() override;
        void enter() override;
        void leave() override;
};