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
 * File:         HT16K33-7Seg.ino
 * Description:  Example using the Seven Segment driver for HT16K33 LED Controller
 *
 * This file is part of SevenSegment
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
 **********************************************************************************/


#include <Wire.h>
#include <SevenSegment.h>

#define DISPLAY_ADDRESS 0x70

#define DEBUG
#define TEST_CODE

#ifdef DEBUG
   // Adapt DebugSerial to the serial device you are using
   #define DebugSerial Serial1
   #define debug_println(a) DebugSerial.println(a);
#else
   #define debug_println(a) ;
#endif

static SevenSegment display = SevenSegment(DISPLAY_ADDRESS);
static uint8_t minutes = 0;
static uint8_t seconds = 0;


void setup()
{
   #ifdef DEBUG
      DebugSerial.begin(19200);
   #endif
   debug_println("");
   debug_println("------------------------");
   debug_println("Seven Segment");

   // put your setup code here, to run once:
   display.begin();

   #ifdef TEST_CODE
      testcode();
   #endif
   display.clearDisplay();
}

void loop()
{
   // Extremely simplistic clock
   while (true)
   {
      display.printTime(minutes, seconds);
      display.writeDisplay();
      delay (500);
      display.toggleColon();
      display.writeColon();
      delay (499);
      display.toggleColon();
      seconds++;
      if (seconds > 59)
      {
         seconds = 0;
         minutes++;
         if (minutes > 99)   minutes = 0;
      }
   }
}

#ifdef TEST_CODE
void testcode()
{
   display.clearDisplay();

   // Test display
   debug_println("Display test");
   display.drawDigit(0, 8);
   display.drawDigit(1, 8);
   display.drawDigit(2, 8);
   display.drawDigit(3, 8);
   display.drawColon();
   display.writeDisplay();
   delay (2000);
   display.drawDigit(0, 0);
   display.drawDigit(1, 1);
   display.drawDigit(2, 2);
   display.drawDigit(3, 3);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.drawDigit(0, 0x0A);
   display.drawDigit(1, 0x0B);
   display.drawDigit(2, 0x0C);
   display.drawDigit(3, 0x0D);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.clearDisplay();

   // Test numbers
   debug_println("Number test");
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.printNumber(5);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(56);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(567);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(5678);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(56789);  // Overflow
   display.toggleColon();
   display.writeDisplay();
   delay (2000);

   display.printNumber(3, DEC, true);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(34, DEC, true);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(345, DEC, true);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(3456, DEC, true);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(34567, DEC, true);  // Overflow
   display.toggleColon();
   display.writeDisplay();
   delay (2000);

   display.printNumber(-5);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(-54);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(-543);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(-5432);  // Display as 5432.
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(-54321);  // Underflow
   display.toggleColon();
   display.writeDisplay();
   delay (2000);

   display.printNumber(-9, DEC, true);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(-98, DEC, true);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(-987, DEC, true);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(-9876, DEC, true);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);
   display.printNumber(-98765, DEC, true);  // Underflow
   display.toggleColon();
   display.writeDisplay();
   delay (2000);

   display.printNumber(0xBCDE, HEX);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);

   display.printNumber(-0xFEDC, HEX);
   display.toggleColon();
   display.writeDisplay();
   delay (2000);

   // Test time
   debug_println("Time test");
   display.printTime(23, 58);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   display.printTime(23, 59);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   display.printTime(00, 00);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   display.toggleColon();
   display.writeDisplay();
   for (int8_t i = 5; i >= 0; i--)
   {
      display.printTime(99, i);
      display.toggleColon();
      display.writeDisplay();
      delay (1000);
   }

   // Test brightness
   debug_println("Brightness test");
   display.drawDigit(0, 8);
   display.drawDigit(1, 8);
   display.drawDigit(2, 8);
   display.drawDigit(3, 8);
   display.toggleColon();
   display.writeDisplay();
   delay (500);
   for (uint8_t i = ht16k33::minBrightness; i <= ht16k33::maxBrightness; i++)
   {
      display.setBrightness(i);
      display.toggleColon();
      display.writeColon();
      delay(500);
   }
   display.clearDigits();

   // Test hyphen
   debug_println("Hyphen test");
   display.writeDisplay();
   for (uint8_t i = 0; i < SevenSegment::displayDigits; i++)
   {
      display.drawHyphen(i);
      display.toggleColon();
      display.writeDisplay();
      delay(1000);
   }
   for (uint8_t i = 0; i < display.displayDigits; i++)
   {
      display.clearDigit(i);
      display.toggleColon();
      display.writeDisplay();
      delay(1000);
   }

   // Test dots
   debug_println("Dot test");
   display.drawDigit(0, 3);
   display.drawDigit(1, 2);
   display.drawDigit(2, 1);
   display.drawDigit(3, 0);
   display.writeDisplay();
   delay(500);
   for (uint8_t i = 0; i < SevenSegment::displayDigits; i++)
   {
      display.drawDot(i);
      display.toggleColon();
      display.writeDisplay();
      delay(1000);
   }
   for (uint8_t i = 0; i < display.displayDigits; i++)
   {
      display.toggleDot(i);
      display.toggleColon();
      display.writeDisplay();
      delay(1000);
   }
   display.clearDigits();

   // Test over and under
   debug_println("Over - Under test");
   display.writeDisplay();
   delay(500);
   for (uint8_t i = 0; i < SevenSegment::displayDigits; i++)
   {
      display.drawOver(i);
      display.toggleColon();
      display.writeDisplay();
      delay(1000);
   }
   for (uint8_t i = 0; i < display.displayDigits; i++)
   {
      display.drawUnder(i);
      display.toggleColon();
      display.writeDisplay();
      delay(1000);
   }
   display.clearDigits();

   // Test lines
   debug_println("Line test");
   display.writeDisplay();
   delay(500);
   for (uint8_t i = 0; i < 5; i++)
   {
      display.toggleColon();
      display.drawLineUpper();
      display.writeDisplay();
      delay(500);
      display.drawLineMiddle();
      display.writeDisplay();
      delay(500);
      display.drawLineLower();
      display.writeDisplay();
      delay(500);
   }
   display.toggleColon();
   display.drawLines2();
   display.writeDisplay();
   delay(1500);
   display.toggleColon();
   display.drawLines3();
   display.writeDisplay();
   delay(1500);
   display.clearDigits();

   // Test blink
   debug_println("Blink test");
   display.printNumber(8888);
   display.writeDisplay();
   for (uint8_t i = ht16k33::blink_Off; i <= ht16k33::blink_0_5Hz; i++)
   {
      display.setBlinkRate(i);
      display.toggleColon();
      display.writeColon();
      delay(5000);
   }
   display.clearDigit(1);
   display.setBlinkRate(ht16k33::blink_Off);

   display.writeDisplay();
}
#endif // TEST_CODE
