#pragma once

#define EEPROM_ADDRESS 0

#define ON_TEMP_MIN 70
#define ON_TEMP_MAX 120
#define ON_TEMP_DEFAULT 100

#define OFF_TEMP_MIN 65
#define OFF_TEMP_MAX 90
#define OFF_TEMP_DEFAULT 80

#define TEN_PERCENT_INC_DEC_DEGREE_MIN 0
#define TEN_PERCENT_INC_DEC_DEGREE_MAX 25
#define TEN_PERCENT_INC_DEGREE_DEFAULT 5
#define TEN_PERCENT_DEC_DEGREE_DEFAULT 15

enum OperationMode { manual, automatic };

struct StoredData
{
    OperationMode mode;
    int  manualModeFanSpeed;
    int  onTemperture;
    int  offTemperture;
    int  tenPercentIncreaseDegree;
    int  tenPercentDecreaseDegree;
};

class StoredDataManager 
{
private:
    StoredData _storedData;
    
    void validateStoredData();
public:
    StoredDataManager();
    void init();
    void save();
    OperationMode getMode();
    int getManualModeFanSpeed();
    int getOnTemperture();
    int getOffTemperture();
    int getTenPercentIncreaseDegree();
    int getTenPercentDecreaseDegree();

    void setManualModeFanSpeed(int fanSpeed);
    void setMode(OperationMode mode);
    void setOnTemperture(int temp);
    void setOffTemperture(int temp);
    void setTenPercentIncreaseDegree(int degree);
    void setTenPercentDecreaseDegree(int degree);
};