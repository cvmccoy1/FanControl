#pragma once

#include "basePidSetupState.h"

class PidProportionSetupState : public BasePidSetupState
{
    public:
        PidProportionSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BasePidSetupState(pidProportionalSetup, lcd, storedDataManager, PID_PROPORTIONAL_MIN, PID_PROPORTIONAL_MAX)
        {
        }
};