#pragma once

#include <PciListener.h>

#define CHANGEKIND_HIGH_TO_LOW LOW
#define CHANGEKIND_LOW_TO_HIGH HIGH

/**
 * A basic PinChangeInterrupt listener implementation.
 */
class MyPciListenerImp : public PciListener
{
    public:
    /**
     * For the constructor add a function callback, that will be called either with CHANGEKIND_HIGH_TO_LOW or CHANGEKIND_LOW_TO_HIGH.
     */
        MyPciListenerImp(byte pin, void (*callback)(byte pin, byte changeKind), bool pullUp = false)
        {
            pciPin = pin;
            _callback = callback;
            
            if (pullUp) 
            {
                pinMode(pin, INPUT_PULLUP);
            } 
            else 
            {
                pinMode(pin, INPUT);
            }
            _lastVal = digitalRead(pciPin);      
        }
        void pciHandleInterrupt(byte vector) override
        {
            byte val = digitalRead(pciPin);
            if(val != _lastVal) 
            {
                _lastVal = val;
                _callback(pciPin, val);
            }
        }
    private:
        void (*_callback)(byte pin, byte changeKind);
	    byte _lastVal;
};