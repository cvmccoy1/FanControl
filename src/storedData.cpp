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
}

void StoredDataManager::save()
{
    EEPROM.put(EEPROM_ADDRESS, _storedData);
}

OperationMode StoredDataManager::getMode()           { return _storedData.mode; }
int StoredDataManager::getManualModeFanSpeed()       { return _storedData.manualModeFanSpeed; }
int StoredDataManager::getDesiredTemperature()        { return _storedData.desiredTemperature; }
bool StoredDataManager::getIsRpmsDisplayed()         { return _storedData.isRpmsDisplayed; }

void StoredDataManager::setMode(OperationMode mode)             { _storedData.mode = mode; }
void StoredDataManager::setManualModeFanSpeed(int fanSpeed)     { _storedData.manualModeFanSpeed = fanSpeed; }
void StoredDataManager::setDesiredTemperature(int temp)          { _storedData.desiredTemperature = temp; }
void StoredDataManager::setIsRpmsDisplayed(bool isDisplayed)    { _storedData.isRpmsDisplayed = isDisplayed; }

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
}