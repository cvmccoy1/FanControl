#pragma once

#define EEPROM_ADDRESS 0

#define DESIRED_TEMP_MIN 70
#define DESIRED_TEMP_MAX 120
#define DESIRED_TEMP_DEFAULT 88

enum OperationMode { manual, automatic };

struct StoredData
{
    OperationMode mode;
    int  manualModeFanSpeed;
    int  desiredTemperature;
    bool isRpmsDisplayed;
};

class StoredDataManager 
{
private:
    StoredData _storedData;
    bool _isDirty = false;
    
    void validateStoredData();
public:
    StoredDataManager();
    void init();
    void save();
    OperationMode getMode();
    int getManualModeFanSpeed();
    int getDesiredTemperature();
    bool getIsRpmsDisplayed();

    void setMode(OperationMode mode);
    void setManualModeFanSpeed(int fanSpeed);
    void setDesiredTemperature(int temp);
    void setIsRpmsDisplayed(bool isDisplayed);
};