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
 * File:         SevenSegment.cpp
 * Description:  Seven Segment driver for HT16K33 LED Controller
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
 
 #include "SevenSegment.h"

// Internally the diplaybuffer is used as follows:
//  displaybuffer[0] = leftmost digit (MSD)
//  displaybuffer[1] = second digit
//  displaybuffer[2] = colon
//  displaybuffer[3] = third digit
//  displaybuffer[4] = rightmost digit (LSD)

// Public functions accepting positions use 0..3; skipping the colon
//  position 0 = leftmost digit
//  position 1 = second digit
//  position 2 = third digit
//  position 3 = rightmost digit


// Numbers from 0 to 9 or 0x0 to 0xF
static const uint8_t numbertable[16] =
{
   0x3F, // 0
   0x06, // 1
   0x5B, // 2
   0x4F, // 3
   0x66, // 4
   0x6D, // 5
   0x7D, // 6
   0x07, // 7
   0x7F, // 8
   0x6F, // 9
   0x77, // a
   0x7C, // b
   0x39, // C
   0x5E, // d
   0x79, // E
   0x71  // F
};


//********
// public
//********

SevenSegment::SevenSegment(uint8_t i2c_addr)
{
}
//----------------------------------------------------------

void SevenSegment::writeDisplay()
{
   ht16k33::writeDisplay();
}
//----------------------------------------------------------

void SevenSegment::writeColon()
{
   Wire.beginTransmission(i2c_address);
   Wire.write(colonPosition << 1); // start at address $02

   Wire.write(displaybuffer[colonPosition] & 0xFF);
   Wire.write(displaybuffer[colonPosition] >> 8);  // No high byte

   Wire.endTransmission();
}
//----------------------------------------------------------

void SevenSegment::drawColon(bool status)
{
   displaybuffer[colonPosition] = status ? colonCode : emptyCode;
}
//----------------------------------------------------------

void SevenSegment::toggleColon()
{
   displaybuffer[colonPosition] = displaybuffer[colonPosition] ? emptyCode : colonCode;
}
//----------------------------------------------------------

void SevenSegment::drawDigit(uint8_t pos, uint8_t value, bool dot)
{
   writeDigitRawPos(pos, ((value <= 0x0F) ? numbertable[value] : emptyCode) | (dot ? dotCode : emptyCode));
}
//----------------------------------------------------------

void SevenSegment::clearDigit(uint8_t pos)
{
   writeDigitRawPos(pos, emptyCode);
}
//----------------------------------------------------------

void SevenSegment::clearDigits()
{
   // Clear all digits, leave the colon alone
   for (int i = 0; i < displayDigits; i++)  clearDigit(i);
}
//----------------------------------------------------------

void SevenSegment::drawDot(uint8_t pos, bool dot)
{
   if (pos > (displaybufSize - 1))   return;
   if (pos >= colonPosition)  pos ++;  // convert to raw position
   writeDigitRaw(pos, (displaybuffer[pos] & 0x7F) | (dot ? dotCode : emptyCode));
}
//----------------------------------------------------------

void SevenSegment::toggleDot(uint8_t pos)
{
   if (pos > (displaybufSize - 1))   return;
   if (pos >= colonPosition)  pos ++;  // convert to raw position
   writeDigitRaw(pos, (displaybuffer[pos] & 0x7F) | ((displaybuffer[pos] & dotCode) ? emptyCode : dotCode));
}
//----------------------------------------------------------

void SevenSegment::drawHyphen(uint8_t pos)
{
   writeDigitRawPos(pos, hyphenCode);
}
//----------------------------------------------------------

void SevenSegment::drawOver(uint8_t pos)
{
   writeDigitRawPos(pos, overCode);
}
//----------------------------------------------------------

void SevenSegment::drawUnder(uint8_t pos)
{
   writeDigitRawPos(pos, underCode);
}
//----------------------------------------------------------

void SevenSegment::drawLineUpper()
{
   for (uint8_t i = 0; i < displayDigits; i++) 
      writeDigitRawPos(i, overCode);
}
//----------------------------------------------------------

void SevenSegment::drawLineMiddle()
{
   for (uint8_t i = 0; i < displayDigits; i++) 
      writeDigitRawPos(i, hyphenCode);
}
//----------------------------------------------------------

void SevenSegment::drawLineLower()
{
   for (uint8_t i = 0; i < displayDigits; i++) 
      writeDigitRawPos(i, underCode);
}
//----------------------------------------------------------

void SevenSegment::drawLines2()
{
   for (uint8_t i = 0; i < displayDigits; i++) 
      writeDigitRawPos(i, overCode | underCode);
}
//----------------------------------------------------------

void SevenSegment::drawLines3()
{
   for (uint8_t i = 0; i < displayDigits; i++) 
      writeDigitRawPos(i, overCode | hyphenCode | underCode);
}
//----------------------------------------------------------

bool SevenSegment::printNumber(int32_t number, uint8_t base, bool padding)
{
   int8_t i = displayDigits - 1;
   bool   lastDigitFree = false;
   bool   negative = (number < 0);

   clearDigits();
   if ((base <= 1) || (base > HEX))  return false;
   if (negative)   number = -number;  // Don't complicate things, work with positive numbers
   do
   {
      if ((i == 0) && (number == 0))   lastDigitFree = true;
      drawDigit(i-- , number % base);
      number /= base;
   } while (((number != 0) || padding) && (i >= 0));

   if (i >= 0)   lastDigitFree = true;

   // Check for overflow
   if (number != 0)
   {
      if (negative)
         drawLineLower();
      else
         drawLineUpper();
      return false;
   }

   // Draw sign: on first digit if there is place, or put a dot at the end of the last digit
   // so -123 is displayed as -123 while -1234 is displayed as 1234.
   if (negative)
      if (lastDigitFree)
         drawHyphen(0);
      else
         drawDot(displayDigits - 1);

   return true;
}
//----------------------------------------------------------

bool SevenSegment::printTime(uint8_t first, uint8_t last)
{
   clearDigits();
   if ((first > 99) || (last > 99))  return false;  // 99:99 is the highest allowed

   drawDigit(0, first / 10);
   drawDigit(1, first % 10);
   drawDigit(2, last / 10);
   drawDigit(3, last % 10);
}
//----------------------------------------------------------


//*********
// Private
//*********

void SevenSegment::writeDigitRaw(uint8_t rawpos, uint8_t bitmask)
{
  if (rawpos > displaybufSize)   return;
  displaybuffer[rawpos] = bitmask;
}
//----------------------------------------------------------

void SevenSegment::writeDigitRawPos(uint8_t pos, uint8_t bitmask)
{
   if (pos > (displaybufSize -1))   return;
   if (pos >= colonPosition)  pos ++;  // convert to raw position
   displaybuffer[pos] = bitmask;
}
//----------------------------------------------------------
