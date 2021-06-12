#pragma once

#include "basePidSetupState.h"

class PidIntegralSetupState : public BasePidSetupState
{
    public:
        PidIntegralSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BasePidSetupState(pidIntegralSetup, lcd, storedDataManager, PID_INTEGRAL_MIN, PID_INTEGRAL_MAX)
        {
        }
};