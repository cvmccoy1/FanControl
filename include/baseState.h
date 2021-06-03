#pragma once

#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "storedData.h"

#define LCD_ROWS    2
#define LCD_COLUMNS 16

enum State { manualMode, automaticMode, modeSetup, desiredTempSetup, displayedSetup, exitSetup, numberOfStates };

class BaseState
{
    private:
        bool _firstTime;
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
        static char _line[LCD_ROWS][LCD_COLUMNS+1];
        virtual void display(bool firstTime);
        virtual void updateValues(bool firstTime);
        virtual void retrieveState();
        virtual void saveState();
        virtual void restoreState();

    public:
        BaseState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager, int32_t encoderMinValue, int32_t encoderMaxValue);
        void OnEntry();
        void OnExit();
        void Activity();
};
