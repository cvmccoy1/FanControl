#include "baseSetupModeState.h"

// Public constructor
BaseSetupModeState::BaseSetupModeState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager, int32_t encoderMinValue, int32_t encoderMaxValue) : 
    BaseState(state, lcd, storedDataManager, encoderMinValue, encoderMaxValue)
{

}

// Protected override members
void BaseSetupModeState::display()
{
    strncpy(_line[0], " >>> SET UP <<< ", LCD_COLUMNS);
    BaseState::display();
} 