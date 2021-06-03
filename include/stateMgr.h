#pragma once

#include "manualModeState.h"
#include "automaticModeState.h"

class StateManager
{
    private:
        State _currentState;
        BaseState *_statesObjects[numberOfStates];
    public:
        StateManager() {}
        void init(LiquidCrystal_I2C *lcd, Encoder *encoder, StoredDataManager *storedDataManager, DallasTemperature *sensors)
        {
            _statesObjects[manualMode]    = (BaseState *)(new ManualModeState(lcd, encoder, storedDataManager, sensors));
            _statesObjects[automaticMode] = (BaseState *)(new AutomaticModeState(lcd, encoder, storedDataManager, sensors));
            _currentState = manualMode;
            _statesObjects[_currentState]->OnEntry();
        };
        State Trigger()
        {
            _statesObjects[_currentState]->OnExit();
            switch (_currentState)
            {
                case manualMode:
                    _currentState = automaticMode;
                    break;
                case automaticMode:
                    _currentState = manualMode;
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