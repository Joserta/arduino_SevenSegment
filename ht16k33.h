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
 * File:         ht16k33.h
 * Description:  HT16K33 LED Controller Driver
 *
 * This file is part of SevenSegment
 *
 * Usage
 *  ht16k33 is the base class containing the ht16k33 driver
 *  It is not intended to be used directly, but can be used as a base class for display types, like here a 7-segment display
 *
 *  begin() or begin(displayAddress)
 *     initialize the display: clear the diplay, set maximum brighness, no blinking
 *  clearDisplay()
 * 	   clear the whole display buffer, including the colon. Note that only the buffer is cleared
 *  setDisplayStatus(s)
 *     turn the display on or off, you can use the constants ht16k33::displayOn and ht16k33::displayOff
 *  setBlinkRate(br)
 *     set the display blinkrate: ht16k33::blink_Off, ht16k33::blink_2Hz, ht16k33::blink_1Hz or ht16k33::blink_0_5Hz
 *     for no blinking, 2Hz, 1Hz or 0.5Hz
 *  setBrightness(b)
 *     set the display brightness, from ht16k33::minBrightness to ht16k33::maxBrightness
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

#ifndef HT16K33_H
#define HT16K33_H

#include <Wire.h>


class ht16k33
{
   public:
      static const uint8_t displayOff     = 0x00;
      static const uint8_t displayOn      = 0x01;

      static const uint8_t minBrightness  = 0x00;
      static const uint8_t maxBrightness  = 0x0F;

      static const uint8_t blink_Off      = 0x00;
      static const uint8_t blink_2Hz      = 0x01;  // 2 Hz
      static const uint8_t blink_1Hz      = 0x02;  // 1 Hz
      static const uint8_t blink_0_5Hz    = 0x03;  // 0.5 Hz

      ht16k33(uint8_t i2c_addr = defaultI2C_address);

      void begin();
      void begin(uint8_t i2c_addr);
      void setDisplayStatus(uint8_t s);
      void setBlinkRate(uint8_t br);
      void setBrightness(uint8_t b);
      void clearDisplay();

   protected:
      static const uint8_t defaultI2C_address = 0x70;
      static const uint8_t displaybufSize = 0x08;

      uint16_t displaybuffer[displaybufSize]; 

      uint8_t i2c_address   = defaultI2C_address;
      uint8_t blinkRate     = blink_Off;
      uint8_t displayStatus = displayOff;

      void writeByte(uint8_t b);
      void writeDisplay();

   private:
      static const uint8_t cmd_turnOff      = 0x20;  // Oscillator off, standby mode
      static const uint8_t cmd_turnOn       = 0x21;  // Oscillator on
      static const uint8_t cmd_displaySetup = 0x80;
      static const uint8_t cmd_brightness   = 0xE0;

};

#endif // HT16K33_H
