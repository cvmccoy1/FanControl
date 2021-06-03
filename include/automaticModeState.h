#pragma once

#include <PID_v1.h>
#include "baseNormalModeState.h"

class AutomaticModeState : public BaseNormalModeState
{
    private:
        PID *_myPID = nullptr;

        //PID parameters. Using defaults.
        double kp=4; //2;   //proportional parameter
        double ki=10; //5;  
        //integral parameter
        double kd=2; //1;   //derivative parameter

        // Input: the current temperature
        double currentTemperature;

        // Output: the PWM value (between minimum and maximum PWM).
        double command;
        double commandMin = 0;
        double commandMax = 255;

        //Setpoint: the desired temperture;
        double desiredTemperature;
        
    public:
        AutomaticModeState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseNormalModeState(automaticMode, lcd, storedDataManager)
        {
            _modeName = "Auto";
            desiredTemperature = _storedDataManager->getDesiredTemperature();
            //init PID
            _myPID = new PID(&currentTemperature, &command, &desiredTemperature, kp, ki, kd, REVERSE);
            //turn the PID on
            _myPID->SetMode(AUTOMATIC);
            _myPID->SetOutputLimits(commandMin, commandMax);
        }
        ~AutomaticModeState()
        {
            if (_myPID != nullptr)
                delete _myPID;
        }

        void GetFanSpeedPWM() override
        {
            currentTemperature = _temperature;
            _myPID->Compute();
            _fanSpeedPWM = command;
        }
};