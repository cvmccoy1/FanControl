#pragma once

#include <PID_v1.h>
#include "manualAutomaticModeState.h"

class AutomaticModeState : ManualAutomaticModeState
{
    private:
        PID *_myPID = nullptr;

        //PID parameters. Using defaults.
        double kp=4; //2;   //proportional parameter
        double ki=10; //5;  
        //integral parameter
        double kd=2; //1;   //derivative parameter

        // Input: the current temperture
        double currentTemperture;

        // Output: the PWM value (between minimum and maximum PWM).
        double command;
        double commandMin = 0;
        double commandMax = 255;

        //Setpoint: the desired temperture;
        double desiredTemperture;
        
    public:
        AutomaticModeState(LiquidCrystal_I2C *lcd, Encoder *encoder, StoredDataManager *storedDataManager, DallasTemperature *sensors) :
            ManualAutomaticModeState(true, lcd, encoder, storedDataManager, sensors)
        {
            desiredTemperture = 88; //storedDataManager->getOnTemperture();
            //init PID
            _myPID = new PID(&currentTemperture, &command, &desiredTemperture, kp, ki, kd, REVERSE);
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
            currentTemperture = _temperture;
            _myPID->Compute();
            _fanSpeedPWM = command;
            Serial.print(F("Temperture: "));
            Serial.print(currentTemperture);
            Serial.print(F(" Desired: "));
            Serial.print(desiredTemperture);
            Serial.print(F(" Command: "));
            Serial.println(command);
        }
};