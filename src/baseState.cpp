#include <Encoder.h>
#include "baseState.h"

// Set up the Roto Encoder
#define ROTO_ENCODER_CLK_PIN 2
#define ROTO_ENCODER_DT_PIN 3
Encoder rotoEncoder(ROTO_ENCODER_CLK_PIN, ROTO_ENCODER_DT_PIN);

// Static variable definitions
char BaseState::_line[LCD_ROWS][LCD_COLUMNS+1];
char BaseState::_line_copy[LCD_ROWS][LCD_COLUMNS];

void BaseState::display()
{
    for (int row = 0; row < LCD_ROWS; row++)
    {
        updateDisplay(row);
    }
}

void BaseState::updateDisplay(int row)
{
    for (int column = 0; column < LCD_COLUMNS; column++)
    {
        char displayChar = _line[row][column];
        if (displayChar != _line_copy[row][column])
        {
            _line_copy[row][column] = displayChar;
            _lcd->setCursor(column, row);
            _lcd->write(displayChar);
        }
    }
}

void BaseState::updateValues()
{
    noInterrupts();
    int32_t movement = (rotoEncoder.read() / _encoderAdjustment);
    rotoEncoder.write(0);
    interrupts();
    if (movement != 0)
    {
        _encoderValue += movement;
        if (_encoderValue > _encoderMaxValue)
            _encoderValue = _encoderMaxValue;
        else if (_encoderValue < _encoderMinValue)
            _encoderValue = _encoderMinValue;
    }
}

void BaseState::clearDisplay()
{
    for (int row = 0; row < LCD_ROWS; row++)
    {
        for (int column = 0; column < LCD_COLUMNS; column++)
        {
            _line[row][column] = ' ';
            _line_copy[row][column] = ' ';
        }
    }
    _lcd->clear();
}

void BaseState::enter()  { _encoderAdjustment = 4; }
void BaseState::leave()  { }

BaseState::BaseState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager, int32_t encoderMinValue, int32_t encoderMaxValue)
{
    _state = state;
    _lcd = lcd;
    _storedDataManager = storedDataManager;
    _encoderMinValue = encoderMinValue;
    _encoderMaxValue = encoderMaxValue;
}
void BaseState::OnEntry()
{
    enter();
    noInterrupts();
    rotoEncoder.write(0);
    interrupts();
}
void BaseState::OnExit()
{
    leave();
}
void BaseState::Activity()
{
    updateValues();
    display();
}
