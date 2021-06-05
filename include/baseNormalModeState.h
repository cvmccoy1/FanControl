#pragma once

#include <PCIManager.h>
#include <PciListenerImp.h>
#include "main.h"
#include "baseState.h"

#define NUMBER_OF_FANS 2

enum DisplayState { temperature, rpms, numberOfDisplayStates };

class BaseNormalModeState : public BaseState
{
    private:
        static int _tachCounter[NUMBER_OF_FANS];
        PciListenerImp* _listener[NUMBER_OF_FANS];
        int _calcInterval;
        int _calcCounter;
        DisplayState _displayState;
        bool _isRpmsDisplayed;

        unsigned long calcRPM(byte index);
        static void OnFanTachPinChange(byte fanPin, byte pinState);
        void GetCurrentTemperature();
       

    protected:
        int _temperature;
        int _fanSpeedPWM;
        const char* _modeName;
        virtual void GetFanSpeedPWM() = 0;
    public:
        BaseNormalModeState(State state, LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager);

    protected:
        void display() override;
        void updateValues() override;
        void enter() override;
        void leave() override;
};