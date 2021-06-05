#include <Arduino.h>
#include <EEPROM.h>
#include "storedData.h"

StoredDataManager::StoredDataManager()
{
};

void StoredDataManager::init()
{
    EEPROM.get(EEPROM_ADDRESS, _storedData);
    validateStoredData();
    _isDirty = false;
}

void StoredDataManager::save()
{
    if (_isDirty)
    {
        EEPROM.put(EEPROM_ADDRESS, _storedData);
        _isDirty = false;
    }
}

OperationMode StoredDataManager::getMode()           { return _storedData.mode; }
int StoredDataManager::getManualModeFanSpeed()       { return _storedData.manualModeFanSpeed; }
int StoredDataManager::getDesiredTemperature()       { return _storedData.desiredTemperature; }
bool StoredDataManager::getIsRpmsDisplayed()         { return _storedData.isRpmsDisplayed; }   
int StoredDataManager::getPidProportional()          { return _storedData.pidProportional; }
int StoredDataManager::getPidIntegral()              { return _storedData.pidIntegral; }
int StoredDataManager::getPidDerivative()            { return _storedData.pidDerivative; }


void StoredDataManager::setMode(OperationMode mode)             
{ 
    if (_storedData.mode != mode)
    {
        _storedData.mode = mode;
        _isDirty = true;
    }
}
void StoredDataManager::setManualModeFanSpeed(int fanSpeed)     
{ 
    if (_storedData.manualModeFanSpeed != fanSpeed)
    {
        _storedData.manualModeFanSpeed = fanSpeed; 
        _isDirty = true;
    }
}
void StoredDataManager::setDesiredTemperature(int temp)         
{ 
    if (_storedData.desiredTemperature != temp)
    {
        _storedData.desiredTemperature = temp; 
        _isDirty = true;
    }
}
void StoredDataManager::setIsRpmsDisplayed(bool isDisplayed)    
{ 
    if (_storedData.isRpmsDisplayed != isDisplayed)
    {
        _storedData.isRpmsDisplayed = isDisplayed; 
        _isDirty = true;
    }
}
void StoredDataManager::setPidProportional(int kp)
{ 
    if (_storedData.pidProportional != kp)
    {
        _storedData.pidProportional = kp; 
        _isDirty = true;
    }
}
void StoredDataManager::setPidIntegral(int ki)
{ 
    if (_storedData.pidIntegral != ki)
    {
        _storedData.pidIntegral = ki; 
        _isDirty = true;
    }
}
void StoredDataManager::setPidDerivative(int kd)
{ 
    if (_storedData.pidDerivative != kd)
    {
        _storedData.pidDerivative = kd; 
        _isDirty = true;
    }
}

// Validates the Setup Data values read from the EEPROM.  
// They should only be invalid until the first write to the EEPROM.
void StoredDataManager::validateStoredData()
{
    if (_storedData.mode != manual && _storedData.mode != automatic)
    {
        _storedData.mode = manual;
    }
    if (_storedData.desiredTemperature < DESIRED_TEMP_MIN || _storedData.desiredTemperature > DESIRED_TEMP_MAX)
    {
        _storedData.desiredTemperature = DESIRED_TEMP_DEFAULT;
    }
    if (_storedData.manualModeFanSpeed < 0 || _storedData.manualModeFanSpeed > 100)
    {
        _storedData.manualModeFanSpeed = 50;
    }
    if (_storedData.manualModeFanSpeed < 0 || _storedData.manualModeFanSpeed > 100)
    {
        _storedData.manualModeFanSpeed = 50;
    }
    if (_storedData.pidProportional < PID_PROPORTIONAL_MIN || _storedData.pidProportional > PID_PROPORTIONAL_MAX)
    {
        _storedData.pidProportional = PID_PROPORTIONAL_DEFAULT;
    }
    if (_storedData.pidIntegral < PID_INTEGRAL_MIN || _storedData.pidIntegral > PID_INTEGRAL_MAX)
    {
        _storedData.pidIntegral = PID_INTEGRAL_DEFAULT;
    }
    if (_storedData.pidDerivative < PID_DERIVATIVE_MIN || _storedData.pidDerivative > PID_DERIVATIVE_MAX)
    {
        _storedData.pidDerivative = PID_DERIVATIVE_DEFAULT;
    }    
}