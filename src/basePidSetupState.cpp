#include "basePidSetupState.h"

BasePidSetupState::BasePidSetupState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager, int32_t encoderMinValue, int32_t encoderMaxValue) :
    BaseSetupModeState(state, lcd, storedDataManager, encoderMinValue, encoderMaxValue)
{
    switch(state)
    {
        case pidProportionalSetup:
            _symbol = 'p';
            _getValue = &StoredDataManager::getPidProportional;
            _setValue = &StoredDataManager::setPidProportional;
            break;
        case pidIntegralSetup:
            _symbol = 'i';
            _getValue = &StoredDataManager:: getPidIntegral;
            _setValue = &StoredDataManager:: setPidIntegral;
            break;                    
        case pidDerivativeSetup:
            _symbol = 'd';
            _getValue = &StoredDataManager::getPidDerivative;
            _setValue = &StoredDataManager::setPidDerivative;
            break;
        default:
            // Invalid PID State
            break;
    }
}

void BasePidSetupState::display()
{
    int x = _encoderValue / 10;
    int y = _encoderValue % 10;
    snprintf_P(_line[1], LCD_COLUMNS+1, PSTR("  PID k%c: %2d.%1d  "), _symbol, x, y);
    BaseSetupModeState::display();
}
void BasePidSetupState::enter()
{
    BaseSetupModeState::enter();
    _encoderValue = (_storedDataManager->*_getValue)();
    _encoderAdjustment = 2;
}
void BasePidSetupState::leave()
{
    BaseSetupModeState::leave();
    (_storedDataManager->*_setValue)(_encoderValue);
}
