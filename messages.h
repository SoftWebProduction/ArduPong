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

#ifndef messages_h
#define messages_h

#include "Arduino.h"

enum MESSAGES
{
    msgPONG = 0,
    msgGAMEOVER,
    msgPAUSE
};

prog_char string_0[] PROGMEM = "ArduPong";  // "String 0" etc are strings to store - change to suit.
prog_char string_1[] PROGMEM = "GAME OVER";
prog_char string_2[] PROGMEM = "PAUSE";


PROGMEM const char *string_table[] = 	   // change "string_table" name to suit
{
    string_0,
    string_1,
    string_2
};

char* getString(int pos)
{
    char buffer[100];    // make sure this is large enough for the largest string it must hold
    
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[pos])));
    
    return buffer;
}

#endif
