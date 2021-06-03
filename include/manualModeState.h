#include "baseNormalModeState.h"

class ManualModeState : public BaseNormalModeState
{
    public:
        ManualModeState(LiquidCrystal_I2C *lcd, Encoder *encoder, StoredDataManager *storedDataManager, DallasTemperature *sensors) :
            BaseNormalModeState(manualMode, lcd, encoder, storedDataManager, sensors)
        {
            _modeName = "Manual";
        }
    protected:
        void saveState() override
        {
            if (_storedDataManager->getManualModeFanSpeed() != _encoderValue)
            {
                _storedDataManager->setManualModeFanSpeed(_encoderValue);
                _storedDataManager->save();
            }
            BaseNormalModeState::saveState();
        }
        void restoreState() override
        {
            _encoderValue = _storedDataManager->getManualModeFanSpeed();
            BaseNormalModeState::restoreState();
        }
};