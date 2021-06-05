#pragma once

#include "manualModeState.h"
#include "automaticModeState.h"
#include "modeSetupState.h"
#include "desiredTempSetupState.h"
#include "displayedSetupState.h"

class StateManager
{
    private:
        State _currentState;
        BaseState *_statesObjects[numberOfStates];
        StoredDataManager *_storedDataManager;
    public:
        StateManager() {}
        void init(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager)
        {
            _storedDataManager = storedDataManager;
            _statesObjects[manualMode]       = (BaseState *)(new ManualModeState(lcd, storedDataManager));
            _statesObjects[automaticMode]    = (BaseState *)(new AutomaticModeState(lcd, storedDataManager));
            _statesObjects[modeSetup]        = (BaseState *)(new ModeSetupState(lcd, storedDataManager));
            _statesObjects[desiredTempSetup] = (BaseState *)(new DesiredTempSetupState(lcd, storedDataManager));
            _statesObjects[displayedSetup]   = (BaseState *)(new DisplayedSetupState(lcd, storedDataManager));
            _currentState = (_storedDataManager->getMode() == automatic) ? automaticMode : manualMode;
            _statesObjects[_currentState]->OnEntry();
        };

        State Trigger()
        {
            _statesObjects[_currentState]->OnExit();
            switch (_currentState)
            {
                case manualMode:
                    _currentState = modeSetup;
                    break;
                case automaticMode:
                    _currentState = modeSetup;
                    break;
                case modeSetup:
                    _currentState = desiredTempSetup;
                    break;
                case desiredTempSetup:
                    _currentState = displayedSetup;
                    break;
                case displayedSetup:
                    _storedDataManager->save();  // Last of the Setup states, so save the data to EEPROM
                    _currentState = (_storedDataManager->getMode() == automatic) ? automaticMode : manualMode;
                    break;
                default:
                    break;
            }
            _statesObjects[_currentState]->OnEntry();
            return _currentState;
        }
        void Activity()
        {
            _statesObjects[_currentState]->Activity();
        }
};