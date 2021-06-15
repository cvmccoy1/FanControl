#pragma once

#define EEPROM_ADDRESS 0

#define DESIRED_TEMP_MIN     70
#define DESIRED_TEMP_MAX     120
#define DESIRED_TEMP_DEFAULT 88

#define PID_PROPORTIONAL_MAX       2000
#define PID_PROPORTIONAL_MIN       100
#define PID_PROPORTIONAL_DEFAULT   1000

#define PID_INTEGRAL_MAX           2000
#define PID_INTEGRAL_MIN           0
#define PID_INTEGRAL_DEFAULT       100

#define PID_DERIVATIVE_MAX         1000
#define PID_DERIVATIVE_MIN         0
#define PID_DERIVATIVE_DEFAULT     100

enum OperationMode { manual, automatic };

struct StoredData
{
    OperationMode mode;
    int  manualModeFanSpeed;
    int  desiredTemperature;
    bool isRpmsDisplayed;
    int  pidProportional;
    int  pidIntegral;
    int  pidDerivative;
};

class StoredDataManager 
{
private:
    StoredData _storedData;
    bool _isDirty = false;
    
    void validateStoredData();
public:
    StoredDataManager();
    void Save();
    OperationMode getMode();
    int getManualModeFanSpeed();
    int getDesiredTemperature();
    bool getIsRpmsDisplayed();
    int getPidProportional();
    int getPidIntegral();
    int getPidDerivative();

    void setMode(OperationMode mode);
    void setManualModeFanSpeed(int fanSpeed);
    void setDesiredTemperature(int temp);
    void setIsRpmsDisplayed(bool isDisplayed);
    void setPidProportional(int kp);
    void setPidIntegral(int ki);
    void setPidDerivative(int kd);
};