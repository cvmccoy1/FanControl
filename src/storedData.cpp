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
int StoredDataManager::getOnTemperture()             { return _storedData.onTemperture; }
int StoredDataManager::getOffTemperture()            { return _storedData.offTemperture; }
int StoredDataManager::getTenPercentIncreaseDegree() { return _storedData.tenPercentIncreaseDegree; }
int StoredDataManager::getTenPercentDecreaseDegree() { return _storedData.tenPercentDecreaseDegree; }

void StoredDataManager::setMode(OperationMode mode)             { _storedData.mode = mode; }
void StoredDataManager::setManualModeFanSpeed(int fanSpeed)     { _storedData.manualModeFanSpeed = fanSpeed; }
void StoredDataManager::setOnTemperture(int temp)               { _storedData.onTemperture = temp; }
void StoredDataManager::setOffTemperture(int temp)              { _storedData.offTemperture = temp; }
void StoredDataManager::setTenPercentIncreaseDegree(int degree) { _storedData.tenPercentIncreaseDegree = degree; }
void StoredDataManager::setTenPercentDecreaseDegree(int degree) { _storedData.tenPercentDecreaseDegree = degree; }

// Validates the Setup Data values read from the EEPROM.  
// They should only be invalid until the first write to the EEPROM.
void StoredDataManager::validateStoredData()
{
    if (_storedData.mode != manual && _storedData.mode != automatic)
    {
        _storedData.mode = manual;
    }
    if (_storedData.onTemperture < ON_TEMP_MIN || _storedData.onTemperture > ON_TEMP_MAX)
    {
        _storedData.onTemperture = ON_TEMP_DEFAULT;
    }
    if (_storedData.offTemperture < OFF_TEMP_MIN || _storedData.offTemperture > OFF_TEMP_MAX)
    {
        _storedData.offTemperture = OFF_TEMP_DEFAULT;
    }
    if (_storedData.tenPercentIncreaseDegree < TEN_PERCENT_INC_DEC_DEGREE_MIN || _storedData.tenPercentIncreaseDegree > TEN_PERCENT_INC_DEC_DEGREE_MAX)
    {
        _storedData.tenPercentIncreaseDegree = TEN_PERCENT_INC_DEGREE_DEFAULT;
    }
    if (_storedData.tenPercentDecreaseDegree < TEN_PERCENT_INC_DEC_DEGREE_MIN || _storedData.tenPercentDecreaseDegree > TEN_PERCENT_INC_DEC_DEGREE_MAX)
    {
        _storedData.tenPercentDecreaseDegree = TEN_PERCENT_DEC_DEGREE_DEFAULT;
    }
}