#pragma once

#include "basePidSetupState.h"

class PidDerivativeSetupState : public BasePidSetupState
{
    public:
        PidDerivativeSetupState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BasePidSetupState(pidDerivativeSetup, lcd, storedDataManager, PID_DERIVATIVE_MIN, PID_DERIVATIVE_MAX)
        {
        }
};