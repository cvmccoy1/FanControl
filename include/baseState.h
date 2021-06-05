#pragma once

#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "storedData.h"

#define LCD_ROWS    2
#define LCD_COLUMNS 16

enum State { manualMode, automaticMode, modeSetup, desiredTempSetup, displayedSetup, pidProportionalSetup, pidIntegralSetup, pidDerivativeSetup, numberOfStates };

class BaseState
{
    private:
        int32_t _encoderMinValue;
        int32_t _encoderMaxValue;
        static char _line_copy[LCD_ROWS][LCD_COLUMNS];
        void updateDisplay(int row);
        void clearDisplay();
    protected:
        LiquidCrystal_I2C *_lcd;
        StoredDataManager *_storedDataManager;
        State _state;
        int32_t _encoderValue;
        int32_t _encoderAdjustment;
        static char _line[LCD_ROWS][LCD_COLUMNS+1];
        virtual void display();
        virtual void updateValues();
        virtual void enter();
        virtual void leave();

    public:
        BaseState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager, int32_t encoderMinValue, int32_t encoderMaxValue);
        void OnEntry();
        void OnExit();
        void Activity();
};
