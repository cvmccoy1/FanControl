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
}