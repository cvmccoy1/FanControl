#pragma once

#include <DallasTemperature.h>
#include <PCIManager.h>
#include <PciListenerImp.h>
#include "main.h"
#include "baseState.h"

#define NUMBER_OF_FANS 2

// Set up the Cooling Fan PWM
#define FAN_PWM_PIN 6

enum DisplayState { temperature, rpms, numberOfDisplayStates };

class BaseNormalModeState : public BaseState
{
    private:
        DallasTemperature *_sensors;
        static int _tachCounter[NUMBER_OF_FANS];
        PciListenerImp* _listener[NUMBER_OF_FANS];
        int _calcInterval;
        int _calcCounter;
        DisplayState _displayState;

        unsigned long calcRPM(byte index);
        static void OnFanTachPinChange(byte fanPin, byte pinState);
        void GetCurrentTemperature();
        virtual void GetFanSpeedPWM();

    protected:
        int _temperature;
        int _fanSpeedPWM;
        const char* _modeName;

    public:
        BaseNormalModeState(State state, LiquidCrystal_I2C *lcd, Encoder *encoder, StoredDataManager *storedDataManager, DallasTemperature *sensors);

    protected:
        void display(bool firstTime) override;
        void updateValues(bool firstTime) override;
        void retrieveState() override;
        void saveState() override;
        void restoreState() override;
};