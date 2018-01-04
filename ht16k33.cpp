/**********************************************************************************
 *
 * Copyright (C) 2018 
 *               Joeri Van hoyweghen
 *               Joserta Consulting & Engineering
 *
 *               All Rights Reserved
 *
 *
 * Contact:      Joeri@Joserta.be
 *
 * File:         ht16k33.cpp
 * Description:  HT16K33 LED Controller Driver
 *
 * This file is part of SevenSegment
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Loosely based upon Adafruit's LEDBackpack library for Adafruit LED Matrix backpacks
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 **********************************************************************************/


#include <Wire.h>
#include "ht16k33.h"


//********
// public
//********

ht16k33::ht16k33(uint8_t i2c_addr)
{
   i2c_address = i2c_addr;
}
//----------------------------------------------------------

void ht16k33::begin(void)
{
   Wire.begin();

   writeByte(cmd_turnOn);          // Turn on oscillator
   clearDisplay();                 // Clear display
   setBlinkRate(blink_Off);        // No blinking
   setBrightness(maxBrightness);   // Max brightness
   setDisplayStatus(displayOn);    // Turn on the display
}
//----------------------------------------------------------

void ht16k33::begin(uint8_t i2c_addr)
{
   i2c_address = i2c_addr;
   begin();
}
//----------------------------------------------------------

void ht16k33::setDisplayStatus(uint8_t s)
{
   displayStatus = s ? displayOn : displayOff;
   writeByte(cmd_displaySetup | displayStatus | blinkRate); 
}
//----------------------------------------------------------

void ht16k33::setBlinkRate(uint8_t br)
{
   if (br > blink_0_5Hz)   br = blink_Off; // turn off if not sure
   blinkRate = br << 1;
   writeByte(cmd_displaySetup | displayStatus | blinkRate); 
}
//----------------------------------------------------------

void ht16k33::setBrightness(uint8_t brightness)
{
   if (brightness > maxBrightness)   brightness = maxBrightness;
   writeByte(cmd_brightness | brightness);
}
//----------------------------------------------------------

void ht16k33::clearDisplay()
{
   // Clear buffer
   for (int i = 0; i < displaybufSize; i++)  displaybuffer[i] = 0x00;
   writeDisplay();  // Write the buffer
}
//----------------------------------------------------------


//***********
// protected
//***********

void ht16k33::writeByte(uint8_t b)
{
   Wire.beginTransmission(i2c_address);
   Wire.write(b);
   Wire.endTransmission();  
}
//----------------------------------------------------------

void ht16k33::writeDisplay()
{
   Wire.beginTransmission(i2c_address);
   Wire.write((uint8_t)0x00); // start at address $00

   for (uint8_t i = 0; i < displaybufSize; i++)
   {
      Wire.write(displaybuffer[i] & 0x00FF);
      Wire.write(displaybuffer[i] >> 8);
   }
   Wire.endTransmission();  
}

