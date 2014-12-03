/*
 Copyright (c) 2013-2014 softWEB Production All Right Reserved, https://swp.pt
 This source is subject to the softWEB Production Licensing models.
 All other rights reserved.
 
 THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 PARTICULAR PURPOSE.
 
 
 Title:		ArduPong
 Descript.  Arduino based Pong like game
 Version:	1.0
 Company:	softWEB Production
 Author:	Isaias Lourenco
 Copyright: softWEB Production © Moerbius, 2014
 License:   MIT
 Website:	https://swp.pt
 
 */

#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button
{
public:
    Button(int pin);
    bool isPressed();
    void setDebounce(int millis);
    
private:
    int _lastState;
    int _pin;
    int _debounce;
    unsigned long _lastDebounce;
};

#endif
