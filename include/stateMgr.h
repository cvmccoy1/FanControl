#pragma once

#include "manualModeState.h"
#include "automaticModeState.h"
#include "operationalModeSetupState.h"
#include "desiredTempSetupState.h"
#include "displayRpmsSetupState.h"
#include "pidProportionalSetupState.h"
#include "pidIntegralSetupState.h"
#include "pidDerivativeSetupState.h"

class StateManager
{
    private:
        State _currentState;
        BaseState *_statesObjects[numberOfStates];
        StoredDataManager *_storedDataManager;

        void lastState()
        {
            _storedDataManager->save();  // Last of the Setup states, so save the data to EEPROM
            _currentState = (_storedDataManager->getMode() == automatic) ? automaticMode : manualMode;            
        }
    public:
        StateManager() {}
        void init(LiquidCrystal_I2C *lcd, StoredDataManager *storedDataManager)
        {
            _storedDataManager = storedDataManager;
            _statesObjects[manualMode]           = (BaseState *)(new ManualModeState(lcd, storedDataManager));
            _statesObjects[automaticMode]        = (BaseState *)(new AutomaticModeState(lcd, storedDataManager));
            _statesObjects[modeSetup]            = (BaseState *)(new OperationModeSetupState(lcd, storedDataManager));
            _statesObjects[desiredTempSetup]     = (BaseState *)(new DesiredTempSetupState(lcd, storedDataManager));
            _statesObjects[displayedSetup]       = (BaseState *)(new DisplayRpmsSetupState(lcd, storedDataManager));
            _statesObjects[pidProportionalSetup] = (BaseState *)(new PidProportionalSetupState(lcd, storedDataManager));
            _statesObjects[pidIntegralSetup]     = (BaseState *)(new PidIntegralSetupState(lcd, storedDataManager));
            _statesObjects[pidDerivativeSetup]   = (BaseState *)(new PidDerivativeSetupState(lcd, storedDataManager));
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
                    if (_storedDataManager->getMode() == automatic)
                    {
                        _currentState = pidProportionalSetup;
                    }
                    else
                    {
                        lastState();
                    }
                    break;
                case pidProportionalSetup:
                    _currentState = pidIntegralSetup;
                    break;
                case pidIntegralSetup:
                    _currentState = pidDerivativeSetup;
                    break;
                case pidDerivativeSetup:
                    lastState();
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