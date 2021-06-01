#pragma once

#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "storedData.h"

#define LCD_ROWS    2
#define LCD_COLUMNS 16

enum State { manualMode, automaticMode, modeSetup, onTempSetup, offTempSetup, tenPercentIncreaseSetup, tenPercentDecreaseSetup, exitSetup, numberOfStates };

class BaseState
{
    private:
        int32_t _encoderMinValue;
        int32_t _encoderMaxValue;
        static char _line_copy[LCD_ROWS][LCD_COLUMNS];
        void updateDisplay(int row);
    protected:
        LiquidCrystal_I2C *_lcd;
        StoredDataManager *_storedDataManager;
        Encoder *_encoder;
        State _state;
        int32_t _encoderValue;
        static char _line[LCD_ROWS][LCD_COLUMNS+1];
        virtual void display();
        virtual void updateValues();
        virtual void retrieveState();
        virtual void saveState();
        virtual void restoreState();

    public:
        BaseState(State state, LiquidCrystal_I2C *lcd, Encoder *encoder, StoredDataManager *storedDataManager, int32_t encoderMinValue, int32_t encoderMaxValue);
        void OnEntry();
        void OnExit();
        void Activity();
};
