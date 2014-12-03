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

#include "Button.h"

Button::Button(int pin)
{
    this->_pin = pin;
    
    //Coloca o pin como input
    pinMode(this->_pin, INPUT);
    
    //Ativa o internal pull-up
    digitalWrite(this->_pin, HIGH);
    
    this->_lastState = false;
    this->_debounce = 50;
    this->_lastDebounce = 0;
}

bool Button::isPressed()
{
    this->_lastState = digitalRead(this->_pin);

    if((millis() - this->_lastDebounce) > this->_debounce)
    {
        bool reading = digitalRead(this->_pin);
        
        if(this->_lastState != reading)
        {
            this->_lastState = !this->_lastState;
        }
        
        this->_lastDebounce = millis();
        
    }
    
    return  _lastState;
    
}

void Button::setDebounce(int millis)
{
    _debounce = millis;
}


