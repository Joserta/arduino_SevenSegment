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
 * File:         SevenSegment.h
 * Description:  Seven Segment driver for HT16K33 LED Controller
 *
 * This file is part of SevenSegment
 *
 * Usage:
 *  General:
 *   The display has 4 digits and a colon: 00:00, with optional dots after each digit
 *   You can retrieve the number of digits from the constant SevenSegment::displayDigits
 *   The positions for the display range from 0 (leftmost digit) to 3 (rightmost digit)
 * 
 *   There are only 2 methods that will actually change the display contents: writeDisplay() and writeColon()
 *   All other methods work on an internal buffer
 *   
 *  Initialise with SevenSegment display = SevenSegment() or SevenSegment display = SevenSegment(device_address).
 *  Before using the display call display.begin() or display.begin(device_address). This will initialise the display and set maximum brightness
 *
 *  Update the display
 * ~~~~~~~~~~~~~~~~~~~~
 *   writeDisplay()
 *      write the buffer contents to the display. You need to call this function to display anything
 *   writeColon()
 * 	    write only the colon to ths display. This function does not change the status of the colon.
 *
 *  Clearing the display
 * ~~~~~~~~~~~~~~~~~~~~~~
 *   clearDisplay()
 * 	    clear the whole display buffer, including the colon. Note that only the buffer is cleared
 *   clearDigits()
 *      clear the digits, but leave the colon alone. Note that only the buffer is cleared
 *
 *  Raw digit handling
 * ~~~~~~~~~~~~~~~~~~~~
 *   drawDigit(pos, value, dot = false)
 *      write a digit with 'value' at position 'pos', optionally yoy can put a dot behind the digit
 *   clearDigit( pos)
 *      clear the digit at position 'pos', including any dot
 *   drawDot(pos, dot = true)
 *      draw a dot at position 'pos', leaving the digit at that position alone
 *   toggleDot(pos)
 *      toggle the dot at position 'pos', leaving the digit at that position alone
 *
 *  Colon handling
 * ~~~~~~~~~~~~~~~~
 *  drawColon(status = true)
 *     draw or remove the colon, depending on status
 *  toggleColon()
 *     toggle the colon
 *
 *  Hypen, Over(flow) and Under(flow)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  drawHyphen(pos)
 *     draw a hypen at position 'pos'
 *  drawOver(pos);
 *     draw an overline at position 'pos'
 *  drawUnder(pos)
 *     draw an underline at position 'pos'
 *  drawLineUpper()
 *     draw an horizontal line at the top of the display, for example to display an overflow
 *  drawLineMiddle()
 *     draw an horizontal line at the midle of the display
 *  drawLineLower()
 *     draw an horizontal line at the bottom of the display, for example to display an underflow
 *  drawLines2()
 *     draw 2 horizontal lines, 1 at the top and 1 at the bottom
 *  drawLines3()
 *     draw 3 horizontal lines, at the top, middle and bottom
 *
 *  Print methods
 * ~~~~~~~~~~~~~~~
 *  printNumber(number, base = 10, padding = false)
 *     print a number in the specified base. If the number is too big to be displayed an overflow or underflow is displayed
 *     base can be any number between 2 and 16, you can use the Arduino constants DEC, HEX, OCT
 *     negative numbers are preceded by a '-' at the first postion, unless the number is 4 digits, then the last dot will be set
 *        so -123 will be displayed as "-123", while -1234 will be displayed as "1234."
 *     if padding is true the number will be padded with zeros: "0001" or "-001"
 *  printTime(first, last)
 *     print a time (or a date), where first is displayed in the first 2 digits and last in the last 2. The colon is not changed.
 *
 *   Display control    These functions have an immediate effect
 * ~~~~~~~~~~~~~~~~~~
 *  setDisplayStatus(s)
 *     turn the display on or off, you can use the constants SevenSegment::displayOn and SevenSegment::displayOff
 *  setBlinkRate(br)
 *     set the display blinkrate: SevenSegment::blink_Off, SevenSegment::blink_2Hz, SevenSegment::blink_1Hz or SevenSegment::blink_0_5Hz
 *     for no blinking, 2Hz, 1Hz or 0.5Hz
 *  setBrightness(b)
 *     set the display brightness, from SevenSegment::minBrightness to SevenSegment::maxBrightness
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


#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H

#include <Wire.h>
#include <ht16k33.h>

class SevenSegment : public ht16k33
{
   public:
      static const uint8_t displayDigits = 0x04;

      SevenSegment(uint8_t i2c_addr = defaultI2C_address);
      void writeDisplay();

      // Raw digit handling
      void clearDigits();
      void drawDigit(uint8_t pos, uint8_t value, bool dot = false);
      void clearDigit(uint8_t pos);
      void drawDot(uint8_t pos, bool dot = true);
      void toggleDot(uint8_t pos);

      // Colon related
      void writeColon();
      void drawColon(bool status = true);
      void toggleColon();

      // Hypen, Over(flow) and Under(flow)
      void drawHyphen(uint8_t pos);
      void drawOver(uint8_t pos);
      void drawUnder(uint8_t pos);
      void drawLineUpper();
      void drawLineMiddle();
      void drawLineLower();
      void drawLines2();
      void drawLines3();

      // Print functions
      bool printNumber(int32_t number, uint8_t base = 10, bool padding = false);
      bool printTime(uint8_t first, uint8_t last);

   private:
      static const uint8_t emptyCode  = 0x00;
      static const uint8_t hyphenCode = 0x40;
      static const uint8_t overCode   = 0x01;
      static const uint8_t underCode  = 0x08;
      static const uint8_t dotCode    = 0x80;
      static const uint8_t colonCode  = 0x02;  // Only valid at colonPosition

      static const uint8_t displaybufSize = displayDigits;
      static const uint8_t colonPosition  = 0x02;

      void writeDigitRaw(uint8_t rawpos, uint8_t bitmask);
      void writeDigitRawPos(uint8_t rawpos, uint8_t bitmask);
};


#endif // SEVENSEGMENT_H
