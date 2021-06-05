#pragma once

#include "baseState.h"

class BaseSetupModeState : public BaseState
{
    public:
        BaseSetupModeState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager, int32_t encoderMinValue, int32_t encoderMaxValue);

    protected:
        void display() override;
};