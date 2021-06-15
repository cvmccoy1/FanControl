#pragma once

#include <PID_v1.h>
#include "baseNormalModeState.h"

class AutomaticModeState : public BaseNormalModeState
{
    private:
        PID *_fanPIDController = nullptr;

        //PID parameters. Using defaults.
        double kp=2.0;  //2;   //proportional parameter
        double ki=5.0;  //5;   //integral parameter
        double kd=1.0;  //1;   //derivative parameter

        // Input: the current temperature
        double currentTemperature;

        // Output: the PWM value (between minimum and maximum PWM).
        double command;
        double commandMin = 0.0;
        double commandMax = 255.0;

        //Setpoint: the desired temperture;
        double desiredTemperature;

        void dispose()
        {
            if (_fanPIDController != nullptr)
            {
                delete _fanPIDController;
            }
        }

    protected:
        void enter() override
        {
            BaseNormalModeState::enter();
            dispose();  // Clean up any left over PID object
            desiredTemperature = _storedDataManager->getDesiredTemperature();
            kp = (double)_storedDataManager->getPidProportional() / 100.0;
            ki = (double)_storedDataManager->getPidIntegral() / 100.0;
            kd = (double)_storedDataManager->getPidDerivative()/ 100.0;
            /*Serial.print("Setpoint = ");
            Serial.print(desiredTemperature);
            Serial.print("; kp = ");
            Serial.print(kp);
            Serial.print("; ki = ");
            Serial.print(ki);                     
            Serial.print("; kd = ");
            Serial.println(kd); */           
            //init PID
            _fanPIDController = new PID(&currentTemperature, &command, &desiredTemperature, kp, ki, kd, REVERSE);
            //turn the PID on
            _fanPIDController->SetMode(AUTOMATIC);
            _fanPIDController->SetOutputLimits(commandMin, commandMax);
        }      
        void leave() override
        {
            BaseNormalModeState::leave();
            dispose();
        }    
        void GetFanSpeedPWM() override
        {
            currentTemperature = _temperature;
            if (_fanPIDController->Compute())
            {
                _fanSpeedPWM = command;
                /*Serial.print("CurrentTemperature = ");
                Serial.print(currentTemperature); 
                Serial.print("; FanSpeedPWM = ");
                Serial.println(_fanSpeedPWM);*/  
            }
        }
        
    public:
        AutomaticModeState(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager) :
            BaseNormalModeState(automaticMode, lcd, storedDataManager)
        {
            _modeName = "Auto";
        }
        ~AutomaticModeState()
        {
            dispose();
        }
};